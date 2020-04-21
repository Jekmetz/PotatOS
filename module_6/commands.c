#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "commands.h"

static command_t COMMANDS[] = {
        {
                .command_name = "pbsi",
                .function = pbsi_command,
        },
        {
                .command_name = "prd",
                .function = prd_command,
        },
        {
                .command_name = "cd",
                .function = cd_command,
        },
        {
                .command_name = "ls",
                .function = ls_command,
        },
        {
                .command_name = "type",
                .function = type_command,
        },
        {
                .command_name = "rename",
                .function = rename_command,
        },
        {
                .command_name = "move",
                .function = move_command,
        },
        {
                .command_name = "help",
                .function = help_command,
        },
        {
                .command_name = "exit",
                .function = exit_command,
        },
        {0}
};

command_t *search_commands(char *command) {
    command_t *current_command = COMMANDS;

    while (current_command->command_name != NULL) {
        if (strcmp(command, current_command->command_name) == 0) {
            return current_command;
        }

        current_command += 1;
    }

    return NULL;
}

int help_command(int argc, char **argv) {
    command_t *current_command = COMMANDS;

    while (current_command->command_name != NULL) {
        printf(" - %s\n", current_command->command_name);

        current_command += 1;
    }

    return 0;
}

int pbsi_command(int argc, char **argv) {
    printf("Printing boot sector information.\n");
    return 0;
}

int prd_command(int argc, char **argv) {
    printf("Printing director.\n");
    return 0;
}

int cd_command(int argc, char **argv) {
    printf("Changing directory.\n");
    return 0;
}

int ls_command(int argc, char **argv) {
    printf("Listing directory.\n");
    return 0;
}

int type_command(int argc, char **argv) {
    printf("Getting type of file.\n");
    return 0;
}

int rename_command(int argc, char **argv) {
    printf("Renaming file.\n");
    return 0;
}

int move_command(int argc, char **argv) {
    printf("Moving file.\n");
    return 0;
}

int exit_command(int argc, char **argv) {
    printf("Shutting down.\n");
    exit(0);
    return 0;
}
