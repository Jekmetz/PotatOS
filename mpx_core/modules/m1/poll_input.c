#include "poll_input.h"

#include <core/io.h>
#include <core/serial.h>
#include <system.h>
#include <string.h>

const EscapeCode escape_codes[] = {
    {"A", UP_ARROW},
    {"B", DOWN_ARROW},
    {"C", RIGHT_ARROW},
    {"D", LEFT_ARROW},

    {"1~", HOME},
    {"2~", INSERT},
    {"3~", DELETE},
    {"4~", END},
    {"5~", PAGE_DOWN},
    {"6~", PAGE_UP},

    {"[A", F1},
    {"[B", F2},
    {"[C", F3},
    {"[D", F4},
    {"[E", F5},
    {"17~", F6},
    {"18~", F7},
    {"19~", F8},
    {"20~", F9},
    {"21~", F10},
    {"23~", F11},
    {"24~", F12},

    {"", 0}
};

int input_available();
int wait_for_input(int timeout);
int get_key();

// How many NOP cycles can we count until we get a new byte and consider it
// part of the same control sequence?
const int TOLERANCE = 300;

const char ESC = '\x1B';

// Make the alt flag be the 9th bit. Avoiding any char business
const int ALT_FLAG = 1 << 8;

int poll_input(char* buffer, int* length) {
  int max_length = *length;
  int current_i = 0;

  // Buffer to hold one char and a null byte for passing to serial_print
  char minibuf[2] = {0, 0};

  while (1) {
    int input = get_key();

    if (0x20 <= input && input <= 0x7E && current_i < max_length) {
      char input_ch = (char)input;
      // If the char is printable, add it to the buffer and print
      buffer[current_i] = input_ch;
      current_i += 1;

      minibuf[0] = input_ch;
      serial_print(minibuf);
    } else if (input == 0x7F && current_i > 0) {
      // If it's backspace then delete the last char in the buffer
      current_i -= 1;
      buffer[current_i] = 0;

      // \b moves the cursor one back. So this moves back, overwrites
      // with a space, and then moves back again.
      serial_print("\b \b");
    }

    if (input == 0x0A || input == 0x0D) {
      // 0A is newline, and 0D is carriage return. For some reason
      // when I hit enter I was receiving \r in the OS, so I just
      // check for both here for enter. If we hit enter, we print
      // a newline and return. We also return if we've hit the max
      // length
      serial_print("\n");
      *length = current_i;
      return 0;
    }
  }
}

int get_key() {
  // Wait until input is available
  while (!input_available()) {
    continue;
  }

  char first_char = inb(COM1);

  // If we get a character and it isn't the start of an escape code, then
  // return that character.
  if (first_char != ESC) {
    return first_char;
  }

  // If it was an ESC, then we wait to see if any characters are going to
  // follow it immediately. If not, then we return the escape character
  if (wait_for_input(TOLERANCE) == 0) {
    return first_char;
  }

  char next_char = inb(COM1);

  // Again we wait to see if we're going to get anything else.
  // If we got ESC + char and nothing else, then we got ALT + char
  if (wait_for_input(TOLERANCE) == 0) {
    return ((int) next_char) | ALT_FLAG;
  }

  // If we still have more bytes incoming, then we can put the immediate
  // next one into the buffer and continue to receive more.
  char buffer[8] = {inb(COM1), 0, 0, 0, 0, 0, 0, 0};
  int buf_i = 1;

  // We're going to receive more bytes until we hit the end which is signalled
  // by either ~ or A-Z
  while (!('A' <= buffer[buf_i - 1] && buffer[buf_i - 1] <= 'Z') && buffer[buf_i - 1] != '~') {
    wait_for_input(TOLERANCE);
    buffer[buf_i] = inb(COM1);
    buf_i += 1;
  }

  // Now that we have the escape code, we can compare it to the ones we know
  // and then send the id related to it.
  int escape_code_id = 0;

  for (int i = 0; escape_codes[i].id != 0; i++) {
    if (strcmp(buffer, escape_codes[i].code) == 0) {
      escape_code_id = escape_codes[i].id;
      break;
    }
  }

  // Remove in production lol
  if (escape_code_id == 0) {
    serial_print("\nUNKNOWN ESCAPE CODE: ESC [");
    serial_print(buffer);
  }

  return escape_code_id;
}

int input_available() {
  return inb(COM1 + 5) & 1;
}

// Tries N times to see if input is available, if input was not
// available, 0 is returned. If it was, the remaining try count
// is returned.
int wait_for_input(int timeout) {
  while (timeout > 0 && !input_available()) {
    nop(); // We nop to spend some time doing nothing
    timeout -= 1;
  }

  return timeout;
}
