#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include "command_utils.h"
#include "commands.h"

int main(int argc, char **argv) {
    if (!(argc == 2 || argc == 3)) {
        printf("Usage: %s DISKIMAGE [PATH]\n", argv[0]);
        return 1;
    }

    if (access(argv[1], F_OK) == -1) {
        printf("Error: Diskimage file '%s' could not be found.", argv[1]);
        return 1;
    }


    char *diskimage_filepath = argv[1];
    char *filesystem_path = argv[2];
    // Load entire disk image into array
    // make functions for dealing with accessing disk image
    // Do things in commands

    char command_buffer[512] = {0};
    // We will never need 16 arguments. Maybe we should just malloc.
    char *command_argv[16] = {0};

    while (1) {
        printf("> ");

        // This happens when someone hits CTRL+D to send EOF
        if (fgets(command_buffer, 511, stdin) == NULL) {
            exit_command(0, NULL);
            break;
        }

        char *command = trim_whitespace(command_buffer);

        int command_argc = split_args(command, command_argv);

        if (command_argc == 0) {
            continue;
        }

        // From this point we can just write programs almost exactly like you normally would write desktop cli apps
        // and we just call their function with argc and argv

        command_t *found_command = search_commands(command_argv[0]);
        if (found_command == NULL) {
            printf("Command not found: %s\n", command_argv[0]);
        } else {
            found_command->function(command_argc, command_argv);
        }

        memset(command_argv, 0, sizeof(char *) * 16);
        memset(command_buffer, 0, 512);

        printf("\n");
    }

    return 0;
}
