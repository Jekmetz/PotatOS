/**
* @file file_wrangler.h
*
* @brief Holds all of the prototypes for file_wrangler.c and has BOOTSECTOR struct
*/

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
void loadDir(BYTE** dirp, BYTE *sys, uint32_t startingSec);
void loadCWD(BYTE* whole, uint32_t startingSec);
void loadROOT(BYTE *sys);
void loadEntireSystem(char* filename);
BOOTSECTORSTRUCT *getBootSectorIn();
BYTE* getSystem();
uint32_t getSystemSize();
uint16_t* getDiabetes1();
uint16_t* getDiabetes2();
BYTE* getCWD();
char* getCwdPath();
char* getImagePath();
void setCwdPath(const char* jerry);
void trim (char *dest, char *src);