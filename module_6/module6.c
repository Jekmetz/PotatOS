#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <inttypes.h>
#include <ctype.h>
#include <unistd.h>

char* trim_whitespace(char* str);
int split_args(char* command, char** argv);

int main(int argc, char** argv) {
    if (argc != 2) {
        printf("Usage: %s DISKIMAGE\n", argv[0]);
        return 1;
    }

    if (access(argv[1], F_OK) == -1 ) {
        printf("Error: Diskimage file '%s' could not be found.", argv[1]);
        return 1;
    }

    char command_buffer[512] = {0};
    char* command_argv[16] = {0};

    while (1) {
        printf("> ");

        fgets(command_buffer, 511, stdin);
        char* command = trim_whitespace(command_buffer);
        printf("You said: \"%s\"\n", command);

        int command_argc = split_args(command, argv);

        if (command_argc == 0) {
            continue;
        }

        // From this point we can just write programs almost exactly like you normally would write desktop cli apps
        // and we just call their function with argc and argv

        for (int i = 0; i < command_argc; i++) {
            puts(command_argv[i]);
        }

        if (strcmp("exit", command_argv[0]) == 0) {
            break;
        }

        memset(command_argv, 0, sizeof(char*) * 16);
        memset(command_buffer, 0, 512);
    }

    return 0;
}

char* trim_whitespace(char* str) {
    while (isspace(*str)) {
        *str = 0;
        str += 1;
    }

    char* end = str + strlen(str) - 1;

    while (isspace(*end)) {
        *end = 0;
        end -= 1;
    }

    return str;
}

int split_args(char* command, char** argv) {
    // Delegate the responsibility of making an array of char pointers to the calling function
    int argc = 0;
    int argv_index = 0;

    bool last_char_was_space = true;

    int command_i = 0;
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
