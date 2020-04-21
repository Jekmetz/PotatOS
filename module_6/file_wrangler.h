#pragma once

#include <stdint.h>
#include <stdio.h>
#include "command_utils.h"

typedef struct BOOTSECTORSTRUCT {
	uint16_t bytesPerSector;
	uint16_t sectorsPerCluster;
	uint16_t numReservedSectors;
	uint16_t numFATCopies;
	uint16_t maxNumRoot;
	uint16_t numOfSectors;
	uint16_t numSECTORSPERFAT;
	uint16_t sectorsPerTrack;
	uint16_t numberHeads;
	uint16_t sectorCountFAT32;
	uint16_t bootSig;
	uint32_t volumeID;
	char *volumeLabel;
	char *fileSystemType;
} BOOTSECTORSTRUCT;

void loadBootSector(FILE *fpIn);
uint16_t *loadFAT(BYTE *fpIn, uint32_t startingSector);
void loadCWD(ENTRY* cwd, BYTE* whole, uint32_t startingSec);
void loadEntireSystem(char* filename);
BOOTSECTORSTRUCT *getBootSectorIn();
BYTE* getSystem();
uint16_t* getDiabetes1();
uint16_t* getDiabetes2();
ENTRY* getCWD();
void trim (char *dest, char *src);