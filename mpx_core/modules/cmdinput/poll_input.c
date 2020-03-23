/**
* @file poll_input.c
*
* @brief The polling input file that allows user input
*/

#include "poll_input.h"
#include "../mpx_supt.h"

#include <core/io.h>
#include <core/serial.h>
#include <string.h>
#include <system.h>

#define BUFFER_LEN 100

// Internally used functions, not placed in the header file for external use
int input_available();
int wait_for_input(int timeout);
int get_key();
void move_cursor(int n);
void print_after_cursor(const char* str);
void delete_after_cursor();
void memcpy(char* destination, const char* source, int n);

/**
* @brief A collection of known control sequences and what they mean.
*
* Control sequences are used to encode special input keys from the
* keyboard that aren't just a one byte character. They start with
* ESCAPE [ and then a series of characters. This array holds the
* series of characters that comes after the bracket, along with the
* corresponding keyboard input. The keyboard inputs are from the KEYS
* enum.
*
*/
const ControlSequence control_sequences[] = {
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

/**
* @brief Maximum amount of NOP cycles that can occur between two inputs from the same control sequence.
*
* @note This is entirely arbitrary and was just increased until things stopped being weird.
*/
const int TOLERANCE = 300;

/**
* @brief The escape character
*/
const char ESC = '\x1B';

/**
* @brief The bit indicating a key from get_key was held with the ALT key
*/
const int ALT_FLAG = 1 << 8;

/**
 * @brief The array of the last used commands
 */
static char history[11][BUFFER_LEN] = { {0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}, {0} };

/**
 * @brief The current length of the last used commands
 */
static int history_length = 0;

/**
 * @brief Getter function for the command history length
 *
 * @return An int of how long the history currently is. Maxes out at 10
 */
int get_history_length() {
  return history_length;
}

/**
 * @brief Getter function for the command history array
 *
 * @return A char pointer to char pointers; an array of commands.
 */
char (*get_command_history())[11][100] {
  return &history;
}

/**
* @brief Polls COM1 for input and puts it into buffer
*
* An internal history is kept so the user can go through past commands
*
* @param buffer a pointer to the buffer to put the user input into
* @param length a pointer to the length of buffer, will be modified to length of input
*
* @return function status
*/
int poll_input(char* buffer, int* length) {
  int history_index = 0;

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

      print_after_cursor(&buffer[cursor_position]);
    }

      switch (input) {
      case UP_ARROW:
          if (history_index < history_length) {
              if (history_index == 0) {
                  memcpy(history[0], buffer, BUFFER_LEN);
              }

              history_index += 1;

              move_cursor(-cursor_position);
              delete_after_cursor();

              // Copy the history into the buffer and print
              memcpy(buffer, history[history_index], BUFFER_LEN);
              serial_print(buffer);
              cursor_position = strlen(buffer);
              current_length = cursor_position;
          }
        break;

      case DOWN_ARROW:
          if (history_index > 0) {
              history_index -= 1;

              move_cursor(-cursor_position);
              delete_after_cursor();

              // Clear the buffer so we can use it for sprintf
              memset(buffer, 0, max_length);

              // Copy the history into the buffer and print
              memcpy(buffer, history[history_index], BUFFER_LEN);
              serial_print(buffer);
              cursor_position = strlen(buffer);
              current_length = cursor_position;
          }
        break;

      case RIGHT_ARROW:
        if (cursor_position < current_length) {
          cursor_position += 1;
          move_cursor(1);
        }
        break;

      case LEFT_ARROW:
        if (cursor_position > 0) {
          cursor_position -= 1;
          move_cursor(-1);
        }
        break;

      case DELETE:
        // Delete the character after the cursor position
        if (cursor_position < current_length) {
          current_length -= 1;

          for (int i = cursor_position; i <= current_length; i++) {
            buffer[i] = buffer[i + 1];
          }

          print_after_cursor(&buffer[cursor_position]);
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

          move_cursor(-1);
          print_after_cursor(&buffer[cursor_position]);
        }
        break;

      case 0x0A:  // \n
      case 0x0D:  // \r
        // 0A is newline, and 0D is carriage return. For some reason
        // when I hit enter I was receiving \r in the OS, so I just
        // check for both here for enter. If we hit enter, we print
        // a newline and return. We also return if we've hit the max
        // length

        memcpy(history[0], buffer, BUFFER_LEN);

        serial_print("\n");

        // If it was empty, return early and dont add it to history
        if (strlen(history[0]) == 0) {
          return 0;
        }

        for (int i = 10; i > 0; i--) {
            memset(history[i], 0, 100);
            memcpy(history[i], history[i - 1], BUFFER_LEN);
        }

        if (history_length < 10) {
            history_length += 1;
        }

        *length = current_length;
        return 0;
    }
  }
}

/**
* @brief Receives a key press, whether a full control sequence or simple character
*
* Calls inb(COM1) to receive bytes. If a control sequence is detected then it
* is parsed according to the control_sequences array. If it was just a simple
* character like the A key. Then the char is sent as an int. Arrow keys and
* other control sequences are special numbers higher than 255 to differentiate
* themselves from the regular characters. The KEYS enum shows the special characters
*
* @return Returns an int corresponding to the key
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

  for (int i = 0; control_sequences[i].id != 0; i++) {
    if (strcmp(buffer, control_sequences[i].code) == 0) {
      escape_code_id = control_sequences[i].id;
      break;
    }
  }

//  // Remove in production lol
//  if (escape_code_id == 0) {
//    serial_print("\nUNKNOWN ESCAPE CODE: ESC [");
//    serial_print(buffer);
//  }

  return escape_code_id;
}

/**
* @brief Checks for input on COM1
*
* @return 1 if input is available, 0 if it isn't.
*/
int input_available() {
  return inb(COM1 + 5) & 1;
}

/**
* @brief Loops N times to check for input
*
* Calls NOP in a while loop at most `timeout` times until it returns.
*
* @param timeout How many times to loop before we give up
*
* @return how many times were left in the timeout
*/
int wait_for_input(int timeout) {
  while (timeout > 0 && !input_available()) {
    nop(); // We nop to spend some time doing nothing
    timeout -= 1;
  }

  return timeout;
}

/**
* @brief Prints text after the cursor without moving the cursor
*
* @param str A pointer to the string to print out
*/
void print_after_cursor(const char* str) {
    // ESC [s - save cursor position
    // ESC [K - delete line following cursor
    serial_print("\x1B[s\x1B[K");

    serial_print(str);

    // ESC [u - restore cursor position
    serial_print("\x1B[u");
}

/**
* @brief Deletes all text after the cursor
*/
void delete_after_cursor() {
    // ESC [K - delete line following cursor
    serial_print("\x1B[K");
}

/**
* @brief Moves the cursor n characters
*
* @param n How many characters to move the character, can be negative.
*/
void move_cursor(int n) {
    char buffer[16] = {0};

    if (n > 0) {
        // ESC [ n C - Move cursor forward n times
        sprintf(buffer, "\x1B[%dC", n);
        serial_print(buffer);
    } else if (n < 0) {
        // ESC [ n D - Move cursor backwards n times
        sprintf(buffer, "\x1B[%dD", -n);
        serial_print(buffer);
    }
    // When it's zero we don't do anything
}


/**
* @brief Copies n bytes from one buffer to another
*
* @param destination Where to copy the bytes to.
* @param source Where to copy the bytes from.
* @param n How many bytes to copy.
*/
void memcpy(char* destination, const char* source, int n) {
    while (n > 0) {
        *destination = *source;
        destination += 1;
        source += 1;
        n -= 1;
    }
}
