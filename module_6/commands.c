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
    
    // ENTRY* cur;
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
    BYTE* cwdIn = getCWD();

    uint32_t numEntries = *((uint32_t*)cwdIn); //get the number of entries
    cwdIn += sizeof(uint32_t);
    uint32_t curSector = *((uint32_t*)cwdIn);
    cwdIn += sizeof(uint32_t);   //move cwdIn to the start of the entries

    //get a flag ready for if it is a directory
    BYTE isDir = 0;
    ENTRY* cur;

    for(uint32_t i = 0; i < numEntries; i++)    //for the number of entries...
    {
        cur = (ENTRY*)(cwdIn + i*sizeof(ENTRY));
        if( cur->empty != 1 && cur->attributes != 0x0F && !(cur->attributes & 0x02) && !(cur->attributes & 0x08)){
            //if it is not a long file name,
            //is not hidden,
            //and is not empty...
            isDir = cur->attributes&0x10;
            if(isDir) //directory
                printf("\033[32m%s\033[0m",cur->fileName);
            else    //not directory
                printf("%s.%s\n", cur->fileName,cur->extension);

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
    
    BYTE* sys = getSystem();
    uint16_t* FAT = getDiabetes1();
    char* fileName = argv[1];
    char curFileName[13];

    BYTE* cwdIn = getCWD();
    uint32_t numEntries = *((uint32_t*)cwdIn); //get the number of entries
    cwdIn += sizeof(uint32_t);
    uint32_t curSector = *((uint32_t*)cwdIn);
    cwdIn += sizeof(uint32_t);   //move cwdIn to the start of the entries

    ENTRY* cur;
    for(uint32_t i = 0; i<numEntries; i++)
    {
        cur = (ENTRY*)(cwdIn + i*sizeof(ENTRY));
        if(cur->empty == 0)
        {
            //get curFileName
            curFileName[0] = '\0';
            strcat(curFileName, cur->fileName);
            strcat(curFileName,".\0");
            strcat(curFileName,cur->extension);
            if(strcmp(fileName,curFileName) == 0)
            {
                //We have found a match, bois.
                uint16_t curClust = cur->firstLogicalCluster;
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
    BYTE* cwdIn = getCWD();     //get cwdIn
    uint32_t numEntries = *((uint32_t*)cwdIn); //get the number of entries
    cwdIn += sizeof(uint32_t);
    uint32_t curSector = *((uint32_t*)cwdIn);
    cwdIn += sizeof(uint32_t);   //move cwdIn to the start of the entries

    int16_t dotPosition = (int16_t)(strchr(argv[2],'.') - argv[2] + 1);
    char newFileName[8]= {0};
    char newExt[3] = {0};
    unsigned char hasExt = 0;

    if(dotPosition > 0)
    {
        //This has an extension
        hasExt = 1;
        memcpy(newFileName,argv[2],dotPosition - 1);
        memcpy(newExt,argv[2]+dotPosition,3);
    } else
    {
        //This does not have an extension
        hasExt = 0;
        memcpy(newFileName,argv[2],strlen(argv[2]));
    }

    char curFileName[13];
    ENTRY* cur;

    for(uint32_t i = 0; i < numEntries; i++)
    {
        cur = (ENTRY*)(cwdIn + i*(sizeof(ENTRY)));

        if(cur->empty == 0)
        {
            //get curFileName
            curFileName[0] = '\0';
            strcat(curFileName, cur->fileName);
            strcat(curFileName,".\0");
            strcat(curFileName,cur->extension);
            if(strcmp(argv[1],curFileName) == 0)
            {
                BYTE* curEntryLoc = getSystem() + (curSector * SECTORSIZE) + (i*32);
                //We have a match there parnter
                //FileName
                memcpy(curEntryLoc, newFileName, 8);
                if(hasExt)
                {
                    memcpy(curEntryLoc + 8, newExt, 3);
                }
                if(hasExt){}
                break;
            }
        }
        
    }

    //Load the new cwd if success!
    loadCWD(getSystem(),curSector);

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
