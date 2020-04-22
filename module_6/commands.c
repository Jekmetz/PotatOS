#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include "commands.h"
#include "file_wrangler.h"

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
    BOOTSECTORSTRUCT *bootSectorIn = getBootSectorIn();
    printf("Bytes per sector: %d\n", bootSectorIn->sectorsPerCluster);
    printf("Sectors per cluster: %d\n", bootSectorIn->bytesPerSector);
    printf("Number of reserved sectors: %d\n", bootSectorIn->numReservedSectors);
    printf("Number of FAT copies: %d\n", bootSectorIn->numFATCopies);
    printf("Max number of root directory entries: %d\n", bootSectorIn->maxNumRoot);
    printf("Total # of sectors in the file system: %d\n",bootSectorIn->numOfSectors);
    printf("Number of sectors per FAT: %d\n", bootSectorIn->numSECTORSPERFAT);
    printf("Sectors per track: %d\n", bootSectorIn->sectorsPerTrack);
    printf("Number of heads: %d\n", bootSectorIn->numberHeads);
    printf("Total sector count for FAT32: %d\n", bootSectorIn->sectorCountFAT32);
    printf("Boot signature: %02X\n", bootSectorIn->bootSig);
    printf("Volume ID: %d\n", bootSectorIn->volumeID);
    printf("Volume label is: %s\n", bootSectorIn->volumeLabel);
    printf("File System Type: %s\n", bootSectorIn->fileSystemType);
    return 0;
}

int prd_command(int argc, char **argv) {
    printf("Printing root directory.\n");
    //HE
    return 0;
}

int cd_command(int argc, char **argv) {
    printf("Changing directory.\n");
    //HE
    return 0;
}

int ls_command(int argc, char **argv) {
    //Grab the CWD
    ENTRY* cwdIn = getCWD();

    //get a flag ready for if it is a directory
    BYTE isDir = 0;

    for(int i = 0; i< MAXENTRIESPERDIR; i++)    //for the number of entries...
    {
        if( cwdIn[i].empty != 1 && cwdIn[i].attributes != 0x0F && !(cwdIn[i].attributes & 0x02)){
            //if it is not a long file name,
            //is not hidden,
            //and is not empty...
            isDir = cwdIn[i].attributes&0x10;
            if(isDir) //directory
                printf("\033[32m%s\033[0m",cwdIn[i].fileName);
            else    //not directory
                printf("%s.%s\n", cwdIn[i].fileName,cwdIn[i].extension);

        }
    }
    return 0;
}

int type_command(int argc, char **argv) {
    //N
    BYTE* whole = getSystem();
    uint16_t* FAT = getDiabetes1();
    ENTRY* cwdIn = getCWD();
    char* fileName = argv[1];

    // Declaring variables
    char *name, *extension;
    
    if(strstr(fileName, ".") != 0) {
        name = strtok(fileName, ".");
        extension = strtok(NULL, "");
        printf("%s\n", name);
        printf("%s\n", extension);
        
    }
    else{
        printf("You must include the extension with the filename\n");
        return 0;
    }

    for(int i = 0; i<MAXENTRIESPERDIR; i++){
        if(cwdIn[i].empty != 1 && strcmp(cwdIn[i].fileName, fileName) == 0){
            printf("Filename: %s\tFirst logical cluster: %d\tSize: %d\tNeeded jumps: %d\n",
               cwdIn[i].fileName, cwdIn[i].firstLogicalCluster, cwdIn[i].fileSize, cwdIn[i].fileSize / 512);


            int current;
            current = cwdIn[i].firstLogicalCluster;

            char *out;

            out = (char*) malloc(sizeof(char) * cwdIn[i].fileSize);

            char *tempOut;
            tempOut = malloc(sizeof(char) * 512);


            memcpy(tempOut, whole + (cwdIn[i].firstLogicalCluster + 33 - 2) * 512, sizeof(char) * 512);
            strcat(out, tempOut);

            while(current < 0xFF8 && FAT[current] != 0){
                current = FAT[current];
                if(FAT[current] == 0){
                    break;
                }
                memcpy(tempOut, whole + (current + 33 - 2) * 512, sizeof(char) * 512);
                strcat(out, tempOut);
            }

            printf("%s", out);
        }
    }   

    return 0;
}

int rename_command(int argc, char **argv) {
    printf("Renaming file.\n");
    //J
    return 0;
}

int move_command(int argc, char **argv) {
    printf("Moving file.\n");
    return 0;
}

int exit_command(int argc, char **argv) {
    printf("Shutting down.\n");
    //HE
    exit(0);
    return 0;
}
