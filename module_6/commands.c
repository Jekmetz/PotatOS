#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include <termios.h>
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
                .command_name = "root",
                .function = root_command,
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

int root_command(int argc, char **argv) {
    ENTRY* cwdIn = getCWD();
    BYTE* whole = getSystem();

    loadCWD(cwdIn, whole,  19);
    setCwdPath(cwdIn[0].fileName);

    return 0;
}

int cd_command(int argc, char **argv) {
    //Grab the CWD
    ENTRY* cwdIn = getCWD();
    BYTE* whole = getSystem();
    char *title;
    // int place;
    
    if(argv[1] != NULL){

        for(int i = 0; i<MAXENTRIESPERDIR; i++){
            if(cwdIn[i].empty != 1 && strcmp(cwdIn[i].fileName, argv[1]) == 0){
                // printf("Yep: %s\tGo to raw: %d\n", cwdIn[i].fileName, cwdIn[i].firstLogicalCluster);
                loadCWD(cwdIn, whole,  33 + cwdIn[i].firstLogicalCluster - 2);
                char path[9];
                path[0] = '/';
                strcpy(path + 1, cwdIn[i].fileName);
                printf("%s\n", path);               
                setCwdPath(strcat(getCwdPath(), path)); 
            }
        }
    }
    else{
        printf("Have to specify dir to go to next\n");
    }
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
    struct termios old,new;
    tcgetattr(0, &old);
    tcgetattr(0, &new);          /* get current terminal attirbutes; 0 is the file descriptor for stdin */
    new.c_lflag &= ~ICANON;      /* disable canonical mode */
    new.c_cc[VMIN] = 1;          /* wait until at least one keystroke available */
    new.c_cc[VTIME] = 0;         /* no timeout */
    tcsetattr(0, TCSANOW, &new); /* set immediately */
    //N
    BYTE* sys = getSystem();
    uint16_t* FAT = getDiabetes1();
    ENTRY* cwdIn = getCWD();
    char* fileName = argv[1];
    char curFileName[13];

    for(int i = 0; i<MAXENTRIESPERDIR; i++){

        if(cwdIn[i].empty == 0)
        {
            //get curFileName
            curFileName[0] = '\0';
            strcat(curFileName, cwdIn[i].fileName);
            strcat(curFileName,".\0");
            strcat(curFileName,cwdIn[i].extension);
            if(strcmp(fileName,curFileName) == 0)
            {
                //We have found a match, bois.
                uint16_t curClust = cwdIn[i].firstLogicalCluster;
                while(curClust < 0xFF8 && FAT[curClust] != 0) 
                {
                    fwrite(sys+(curClust+31)*512,512,1,stdout);
                    // printf("------Press [SPACE]------");
                    if(argc > 2) getchar();
                    // printf("\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b");
                    curClust = FAT[curClust];
                }
                break;
            }
        }
    }   

    tcsetattr(0, TCSANOW, &old); /* set immediately */    

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
