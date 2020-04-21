#include <stdbool.h>
#include <ctype.h>
#include <string.h>
#include <stdint.h>
#include "command_utils.h"

char *trim_whitespace(char *str) {
    while (isspace(*str)) {
        *str = 0;
        str += 1;
    }

    char *end = str + strlen(str) - 1;

    while (isspace(*end)) {
        *end = 0;
        end -= 1;
    }

    return str;
}

uint32_t split_args(char *command, char **argv) {
    // Delegate the responsibility of making an array of char pointers to the calling function
    uint32_t argc = 0;
    uint32_t argv_index = 0;

    bool last_char_was_space = true;

    uint32_t command_i = 0;
    bool keep_going = true;

    while (keep_going) {
        switch (command[command_i]) {
            case ' ':
                command[command_i] = 0;

                if (!last_char_was_space) {
                    argc += 1;
                    last_char_was_space = true;
                }
                break;

            case 0:
                if (!last_char_was_space) {
                    argc += 1;
                    argv[argv_index] = command + command_i;
                }
                keep_going = false;
                break;

            default:
                if (last_char_was_space) {
                    last_char_was_space = false;
                    argv[argv_index] = command + command_i;
                    argv_index += 1;
                }
                break;
        }

        command_i += 1;
    }

    return argc;
}
