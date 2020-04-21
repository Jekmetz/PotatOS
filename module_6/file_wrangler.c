#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "file_wrangler.h"
#include "command_utils.h"

BOOTSECTORSTRUCT* bootSectorIn;

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

ENTRY *loadCWD(BYTE *whole, uint32_t startingSec){
	// ENTRY array to the cws 
	ENTRY *cwd = malloc(sizeof(ENTRY) * MAXENTRIESPERDIR);

	for(uint32_t i = 0; i<MAXENTRIESPERDIR; i++){

		// printf("%d\t%02X\n",i,  *(whole + (i * 32)));

		char *fileName = malloc(sizeof(char) * 8);
		char *extension = malloc(sizeof(char) * 3);


		// Index is deleted
		if ( *(whole + (startingSec * SECTORSIZE) + (i * 32)) == 0xE5) {
			cwd[i].empty = 1;
		}
		// Last index
		else if ( *(whole + (startingSec * SECTORSIZE) + (i * 32)) == 0x00){
			cwd[i].empty = 1;
			// TODO: Stop 
		}
		else if( *(whole + (startingSec * SECTORSIZE) + (i * 32) + 11) == 0x0F){
			cwd[i].empty = 1;
		}
		else{
				cwd[i].empty = 0;
				memcpy(fileName, whole + (startingSec * SECTORSIZE) + (32 * i) + 0, sizeof(char) * 8);
				trim(fileName,fileName);
				cwd[i].fileName = fileName;
				memcpy(extension, whole + (startingSec * SECTORSIZE) + (32 * i) + 8, sizeof(char) * 3);
				cwd[i].extension = extension;
				
				cwd[i].attributes = 			*((char*)  (whole + (32 * i) + 11)); 
				cwd[i].reserved = 				*((uint16_t*) (whole + (32 * i) + 12));
				cwd[i].creationTime = 			*((uint16_t*) (whole + (32 * i) + 14));
				cwd[i].creationDate = 			*((uint16_t*) (whole + (32 * i) + 16));
				cwd[i].lastAccessDate = 		*((uint16_t*) (whole + (32 * i) + 18));
				cwd[i].lastWriteTime = 			*((uint16_t*) (whole + (32 * i) + 22));
				cwd[i].lastWriteDate = 			*((uint16_t*) (whole + (32 * i) + 24));
				cwd[i].firstLogicalCluster = 	*((uint16_t*) (whole + (32 * i) + 26));
				cwd[i].fileSize = 				*((uint32_t*)   (whole + (32 * i) + 28));
		}
	}

	return cwd;
}

BOOTSECTORSTRUCT* getBootSectorIn()
{
	return bootSectorIn;
}

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