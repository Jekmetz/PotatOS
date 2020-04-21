#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdint.h>
#include <stdlib.h>
#include "command_utils.h"
#include "commands.h"
#include "file_wrangler.h"

uint16_t *fat1, *fat2;

int main(int argc, char **argv) {
    if (!(argc == 2 || argc == 3)) {
        printf("Usage: %s DISKIMAGE [PATH]\n", argv[0]);
        return 1;
    }

    if (access(argv[1], F_OK) == -1) {
        printf("Error: Diskimage file '%s' could not be found.", argv[1]);
        return 1;
    }

    char *filesystem_path = argv[2];
    // Load entire disk image into array
    // make functions for dealing with accessing disk image
    // Do things in commands
    FILE *fp;
    char* filename = argv[1]; 

    fp = fopen(filename, "r");

    fseek(fp, 0, SEEK_END);
    uint32_t size = ftell(fp);
    fseek(fp, 0, SEEK_SET);

    // Size of entire system
    BYTE *ENTIRESYSTEM = malloc(sizeof(char) * size);

    // Reading the ENTIRE DAMN THING
    fread(ENTIRESYSTEM, size, 1, fp);

    // Loading boot sector
    // call with getBootSectorIn();
    loadBootSector(fp);

    // The starting sectors of the two FATs
    uint32_t fat1StartingSec = 1;
    uint32_t fat2StartingSec = 10;

    // Loading both FATs into arrays
    fat1 = loadFAT(ENTIRESYSTEM, fat1StartingSec);
    fat2 = loadFAT(ENTIRESYSTEM, fat2StartingSec); 

    ENTRY *cwd;
    uint32_t rootDirStartingSec = 19;

    cwd = loadCWD(ENTIRESYSTEM, rootDirStartingSec);   

    //TODO REMOVE
    if(cwd == NULL) {};

    if(argc == 2) //Command input mode
    {
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

            uint32_t command_argc = split_args(command, command_argv);

            if (command_argc == 0) {
                continue;
            }

            // From this pouint32_t we can just write programs almost exactly like you normally would write desktop cli apps
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
    }
    else //file input mode
    {
        //recursively go through files and pruint32_t out 
        //info from found file.
        //otherwise, error report
    }

    return 0;
}