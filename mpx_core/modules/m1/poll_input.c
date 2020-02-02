#include "poll_input.h"

#include <core/io.h>
#include <core/serial.h>
#include <string.h>
#include <system.h>

// TODO
/**
* @brief Enter a brief description of the element below
*
* Enter a detailed description of the of the element below
*
* @param param1 Description of param1
* @param param2 Description of param2
*
* @return Description of return
*
* @code
* include code here if you wish
* as many lines as you like
* if(name = nick){
*   look = sexy
* }
* @endcode
*
* @note Something to note
*
* @warning A warning
*/
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
// TODO
/**
* @brief Enter a brief description of the element below
*
* Enter a detailed description of the of the element below
*
* @param param1 Description of param1
* @param param2 Description of param2
*
* @return Description of return
*
* @code
* include code here if you wish
* as many lines as you like
* if(name = nick){
*   look = sexy
* }
* @endcode
*
* @note Something to note
*
* @warning A warning
*/
const int TOLERANCE = 300;

// TODO
/**
* @brief Enter a brief description of the element below
*
* Enter a detailed description of the of the element below
*
* @param param1 Description of param1
* @param param2 Description of param2
*
* @return Description of return
*
* @code
* include code here if you wish
* as many lines as you like
* if(name = nick){
*   look = sexy
* }
* @endcode
*
* @note Something to note
*
* @warning A warning
*/
const char ESC = '\x1B';

// TODO
/**
* @brief Enter a brief description of the element below
*
* Enter a detailed description of the of the element below
*
* @param param1 Description of param1
* @param param2 Description of param2
*
* @return Description of return
*
* @code
* include code here if you wish
* as many lines as you like
* if(name = nick){
*   look = sexy
* }
* @endcode
*
* @note Something to note
*
* @warning A warning
*/
// Make the alt flag be the 9th bit. Avoiding any char business
const int ALT_FLAG = 1 << 8;

// TODO
/**
* @brief Enter a brief description of the element below
*
* Enter a detailed description of the of the element below
*
* @param param1 Description of param1
* @param param2 Description of param2
*
* @return Description of return
*
* @code
* include code here if you wish
* as many lines as you like
* if(name = nick){
*   look = sexy
* }
* @endcode
*
* @note Something to note
*
* @warning A warning
*/
int poll_input(char* buffer, int* length) {
  int max_length = *length;
  int current_length = 0;
  int cursor_position = 0;

  // Buffer to hold one char and a null byte for passing to serial_print
  char minibuf[2] = {0, 0};

  while (1) {
    int input = get_key();

    if (0x20 <= input && input <= 0x7E && current_length < max_length) {
      char input_ch = (char)input;

      // If the char is printable, add it to the buffer and print
      for (int i = current_length; i > cursor_position; i--) {
        buffer[i] = buffer[i - 1];
      }

      buffer[cursor_position] = input_ch;

      current_length += 1;
      cursor_position += 1;

      minibuf[0] = input_ch;
      serial_print(minibuf);
    }

    switch (input) {
      case UP_ARROW:
        break;

      case DOWN_ARROW:
        break;

      case RIGHT_ARROW:
        if (cursor_position < current_length) {
          cursor_position += 1;
          serial_print("\x1B[C");
        }
        break;

      case LEFT_ARROW:
        if (cursor_position > 0) {
          cursor_position -= 1;
          serial_print("\x1B[D");
        }
        break;

      case DELETE:
        // Delete the character after the cursor position
        if (cursor_position < current_length) {
          current_length -= 1;

          for (int i = cursor_position; i <= current_length; i++) {
            buffer[i] = buffer[i + 1];
          }

          // ESC [s - save cursor position
          // ESC [K - delete line following cursor
          serial_print("\x1B[s\x1B[K");

          serial_print(&buffer[cursor_position]);

          // ESC [u - restore cursor position
          serial_print("\x1B[u");
        }
        break;

      case 0x7F:  // Backspace
        // Delete the character before the cursor position
        if (cursor_position > 0 && current_length > 0) {
          current_length -= 1;
          cursor_position -= 1;

          for (int i = cursor_position; i <= current_length; i++) {
            buffer[i] = buffer[i + 1];
          }

          // ESC [D - move cursor back
          // ESC [s - save cursor position
          // ESC [K - delete line following cursor
          serial_print("\x1B[D\x1B[s\x1B[K");

          serial_print(&buffer[cursor_position]);

          // ESC [u - restore cursor position
          serial_print("\x1B[u");
        }
        break;

      case 0x0A:  // \n
      case 0x0D:  // \r
        // 0A is newline, and 0D is carriage return. For some reason
        // when I hit enter I was receiving \r in the OS, so I just
        // check for both here for enter. If we hit enter, we print
        // a newline and return. We also return if we've hit the max
        // length
        serial_print("\n");
        *length = current_length;
        return 0;
    }
  }
}

// TODO
/**
* @brief Enter a brief description of the element below
*
* Enter a detailed description of the of the element below
*
* @param param1 Description of param1
* @param param2 Description of param2
*
* @return Description of return
*
* @code
* include code here if you wish
* as many lines as you like
* if(name = nick){
*   look = sexy
* }
* @endcode
*
* @note Something to note
*
* @warning A warning
*/
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

// TODO
/**
* @brief Enter a brief description of the element below
*
* Enter a detailed description of the of the element below
*
* @param param1 Description of param1
* @param param2 Description of param2
*
* @return Description of return
*
* @code
* include code here if you wish
* as many lines as you like
* if(name = nick){
*   look = sexy
* }
* @endcode
*
* @note Something to note
*
* @warning A warning
*/
int input_available() {
  return inb(COM1 + 5) & 1;
}

// TODO
/**
* @brief Enter a brief description of the element below
*
* Enter a detailed description of the of the element below
*
* @param param1 Description of param1
* @param param2 Description of param2
*
* @return Description of return
*
* @code
* include code here if you wish
* as many lines as you like
* if(name = nick){
*   look = sexy
* }
* @endcode
*
* @note Something to note
*
* @warning A warning
*/
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
