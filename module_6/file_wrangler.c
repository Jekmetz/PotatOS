#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "file_wrangler.h"
#include "command_utils.h"

BOOTSECTORSTRUCT* bootSectorIn;
BYTE* sys;
uint16_t *fat1, *fat2;
BYTE *cwd = NULL, *root = NULL;
/*
CWD is stored:
uint32  :  number of entries
uint32  :  sector
ENTRY   :  ENTRY 1
ENTRY   :  ENTRY 2
...
ENTRY   :  ENTRY (number of entries)
*/
char cwdPath[100] = {0};

void loadBootSector(FILE *fpIn){
    // Loading Bytes Per Sector
    // Starting location 11 bytes
	bootSectorIn = (BOOTSECTORSTRUCT*) malloc(sizeof(BOOTSECTORSTRUCT));
    fseek(fpIn, 0, SEEK_SET);

    char *volumeLabel = malloc(sizeof(char) * 11);
    char *fileSystemType = malloc(sizeof(char) * 8);

    BYTE bs[SECTORSIZE];
    
    fread(bs, SECTORSIZE, 1, fpIn);

    bootSectorIn->bytesPerSector = *((uint16_t*)(bs+11));
    bootSectorIn->sectorsPerCluster = *((char*)(bs+13));
    bootSectorIn->numReservedSectors = *((uint16_t*)(bs+14));
    bootSectorIn->numFATCopies = *((char*)(bs+16));
    bootSectorIn->maxNumRoot = *((uint16_t*)(bs+17));
    bootSectorIn->numOfSectors = *((uint16_t*)(bs+19));
    bootSectorIn->numSECTORSPERFAT = *((uint16_t*)(bs+22));
    bootSectorIn->sectorsPerTrack = *((uint16_t*)(bs+24));
    bootSectorIn->numberHeads = *((uint16_t*)(bs+26));
    bootSectorIn->sectorCountFAT32 = *((uint32_t*)(bs+32));
    bootSectorIn->bootSig = *((char*)(bs+38));
    bootSectorIn->volumeID = *((uint32_t*)(bs+32));
    memcpy(volumeLabel, bs+43, sizeof(char) * 11);
    bootSectorIn->volumeLabel = volumeLabel;
    memcpy(fileSystemType, bs+54, sizeof(char) * 8);
    bootSectorIn->fileSystemType = fileSystemType;

}

uint16_t *loadFAT(BYTE* sys, uint32_t startingSector) { 
    
    //move sys forward to the correct starting position
    sys = (sys + SECTORSIZE * startingSector);

    //fat table malloc
    uint16_t *fatTable = malloc(sizeof(uint16_t) * FATTABLESIZE);

    // For loop to iterate over FAT to insert 12 bit long FAT entry
    BYTE* si;
    //make our phatty bois
    uint16_t boi0,boi1;
    for(uint32_t i = 0; i<  FATTABLESIZE/2;i++){
        
        si = sys + i*3;

        boi0 = ((*(si+1)&0x0F)<<8) + (*(si));
        boi1 = (*(si+2)<<4) + (*(si+1)>>4);

        // printf("---------------\n*(sys+si): %x\n,*(sys+si+1): %x\n,*(sys+si+2): %x\n(*(sys+si+1)|0x0F)<<8: %x\n*(sys+si): %x\nboi0: %x\nboi1: %x\n*(sys+si+2)<<4: %x\n(*(sys+si+1)>>4): %x\n"
        //  ,*(sys+si),*(sys+si+1),*(sys+si+2),(*(sys+si+1)&0x0F)<<8,*(sys+si),boi0,boi1,*(sys+si+2)<<4,(*(sys+si+1)>>4));
        //voila
        
        fatTable[2*i] = boi0;
        fatTable[2*i+1] = boi1;
    }

    return fatTable;
}

void loadCWD(BYTE *sys, uint32_t startingSec){
    //find how many entries we are looking at here:
    BYTE* startingLoc = sys + (startingSec * SECTORSIZE);
    uint32_t numEntries = 0;

    do
    {
        numEntries++;
    } while(*(startingLoc + numEntries*32) != '\0');

    //free it if we have used it before
    if(cwd!=NULL) free(cwd);

    cwd = (BYTE*) malloc(2* sizeof(uint32_t) + numEntries * sizeof(ENTRY));

    //get the first two ints in there
    memcpy(cwd,&numEntries,4);
    memcpy(cwd+4,&startingSec,4);

    BYTE* curLoc;
    ENTRY* curEntry;
    //go through all of the entries
    for(uint32_t i = 0; i < numEntries; i++)
    {
        curLoc = (sys+(startingSec * SECTORSIZE) + (i*32));
        curEntry = (ENTRY*)((cwd+8) + i*sizeof(ENTRY));

        //Index is deleted
        if(*curLoc == 0xE5) 
        {
            curEntry->empty = 1;
        }else if (*curLoc == 0x00) //Last entry (shouldn't happen)
        {
            curEntry->empty = 1;
        } else if (*curLoc == 0x0F) //Something or other
        {
            curEntry->empty = 1;
        }else //If we have ourselves a rowdy one...
        {   
            //wipe out filename and extension
            memset(curEntry->fileName, 0, 13);

            curEntry->empty = 0;
            memcpy(curEntry->fileName, curLoc + 0, 8);
            trim_whitespace(curEntry->fileName);
            (curEntry->fileName)[8] = '\0';
            memcpy(curEntry->extension, curLoc + 8, 3);
            (curEntry->extension)[8] = '\0';

            curEntry->attributes =             *((char*)     (curLoc + 11)); 
            curEntry->reserved =               *((uint16_t*) (curLoc + 12));
            curEntry->creationTime =           *((uint16_t*) (curLoc + 14));
            curEntry->creationDate =           *((uint16_t*) (curLoc + 16));
            curEntry->lastAccessDate =         *((uint16_t*) (curLoc + 18));
            curEntry->lastWriteTime =          *((uint16_t*) (curLoc + 22));
            curEntry->lastWriteDate =          *((uint16_t*) (curLoc + 24));
            curEntry->firstLogicalCluster =    *((uint16_t*) (curLoc + 26));
            curEntry->fileSize =               *((uint32_t*) (curLoc + 28));
        }

    }
}

void loadROOT(BYTE *sys){
    //find how many entries we are looking at here:
    int startingSec = 19;
    BYTE* startingLoc = sys + (startingSec * SECTORSIZE);
    uint32_t numEntries = 0;

    do
    {
        numEntries++;
    } while(*(startingLoc + numEntries*32) != '\0');

    //free it if we have used it before
    if(root!=NULL) free(root);

    root = (BYTE*) malloc(2* sizeof(uint32_t) + numEntries * sizeof(ENTRY));

    //get the first two ints in there
    memcpy(root,&numEntries,4);
    memcpy(root+4,&startingSec,4);

    BYTE* curLoc;
    ENTRY* curEntry;
    //go through all of the entries
    for(uint32_t i = 0; i < numEntries; i++)
    {
        curLoc = (sys+(startingSec * SECTORSIZE) + (i*32));
        curEntry = (ENTRY*)((root+8) + i*sizeof(ENTRY));

        //Index is deleted
        if(*curLoc == 0xE5) 
        {
            curEntry->empty = 1;
        }else if (*curLoc == 0x00) //Last entry (shouldn't happen)
        {
            curEntry->empty = 1;
        } else if (*curLoc == 0x0F) //Something or other
        {
            curEntry->empty = 1;
        }else //If we have ourselves a rowdy one...
        {   
            //wipe out filename and extension
            memset(curEntry->fileName, 0, 13);

            curEntry->empty = 0;
            memcpy(curEntry->fileName, curLoc + 0, 8);
            trim_whitespace(curEntry->fileName);
            (curEntry->fileName)[8] = '\0';
            memcpy(curEntry->extension, curLoc + 8, 3);
            (curEntry->extension)[8] = '\0';

            curEntry->attributes =             *((char*)     (curLoc + 11)); 
            curEntry->reserved =               *((uint16_t*) (curLoc + 12));
            curEntry->creationTime =           *((uint16_t*) (curLoc + 14));
            curEntry->creationDate =           *((uint16_t*) (curLoc + 16));
            curEntry->lastAccessDate =         *((uint16_t*) (curLoc + 18));
            curEntry->lastWriteTime =          *((uint16_t*) (curLoc + 22));
            curEntry->lastWriteDate =          *((uint16_t*) (curLoc + 24));
            curEntry->firstLogicalCluster =    *((uint16_t*) (curLoc + 26));
            curEntry->fileSize =               *((uint32_t*) (curLoc + 28));
        }

    }
}

void loadEntireSystem(char* filename)
{
    //init
    strcat(cwdPath,"/");

	FILE *fp;

    fp = fopen(filename, "r");

    fseek(fp, 0, SEEK_END);
    uint32_t size = ftell(fp);
    fseek(fp, 0, SEEK_SET);

    // Size of entire system
    sys = malloc(sizeof(char) * size);

    // Reading the ENTIRE DAMN THING
    fread(sys, size, 1, fp);

    // Loading boot sector
    // call with getBootSectorIn();
    loadBootSector(fp);

    // The starting sectors of the two FATs
    uint32_t fat1StartingSec = 1;
    uint32_t fat2StartingSec = 10;

    // Loading both FATs into arrays
    fat1 = loadFAT(sys, fat1StartingSec);
    fat2 = loadFAT(sys, fat2StartingSec); 

    uint32_t rootDirStartingSec = 19;

    loadCWD(sys, rootDirStartingSec);
    loadROOT(sys);
    setCwdPath(((ENTRY*)(cwd+8))->fileName);

    fclose(fp);
}

BOOTSECTORSTRUCT* getBootSectorIn() { return bootSectorIn; }

BYTE* getSystem() {	return sys; }

uint16_t* getDiabetes1() { return fat1; }

uint16_t* getDiabetes2() { return fat2; }

BYTE* getCWD() { return cwd; }

char* getCwdPath() { return cwdPath; }

BYTE* getRoot(){ return root; }

void setCwdPath(const char* jerry) { memcpy(cwdPath, jerry, sizeof(cwdPath)); } // Changed this: strlen(jerry)

void trim (char *dest, char *src)
{
    if (!src || !dest)
       return;

    int len = strlen (src);

    if (!len) {
        *dest = '\0';
        return;
    }
    char *ptr = src + len - 1;

    // remove trailing whitespace
    while (ptr > src) {
        if (!isspace (*ptr))
            break;
        ptr--;
    }

    ptr++;

    char *q;
    // remove leading whitespace
    for (q = src; (q < ptr && isspace (*q)); q++)
        ;

    while (q < ptr)
        *dest++ = *q++;

    *dest = '\0';
}