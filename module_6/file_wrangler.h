/**
* @file file_wrangler.h
*
* @brief Holds all of the prototypes for file_wrangler.c and has BOOTSECTOR struct
*/

#pragma once

#include <stdint.h>
#include <stdio.h>
#include "command_utils.h"

// TODO: Build doxygen entry for this struct
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

// TODO: Doxy
void loadBootSector(FILE *fpIn);

/**
* @brief loads the FAT
*
* @param sys raw byte array of entire file system
* @param startingSector number of the sector to start loading the fat from
*
* @return pointer to FAT
*/
uint16_t *loadFAT(BYTE *fpIn, uint32_t startingSector);

// TODO: Doxy
void loadDir(BYTE** dirp, BYTE *sys, uint32_t startingSec);

/**
* @brief loads the current working directory into global BYTE* cwd
*
* @param sys raw byte array of entire file system
* @param startingSector number of the sector to start loading the cwd from
*/
void loadCWD(BYTE* whole, uint32_t startingSec);

/**
* @brief loads the root into global BYTE* root
*
* @param sys raw byte array of entire file system
*/
void loadROOT(BYTE *sys);

/**
* @brief loads the entire system
*
* @param filename filename of the correct FAT12 Image
*/
void loadEntireSystem(char* filename);

/**
* @brief gets boot sector in
*
* @return pointer to boot sector in
*/
BOOTSECTORSTRUCT *getBootSectorIn();

/**
* @brief gets entire system pointer
*
* @return pointer to entire system
*/
BYTE* getSystem();

// TODO: Doxy
uint32_t getSystemSize();

/**
* @brief gets fat table 1
*
* @return pointer to fat table 1
*/
uint16_t* getDiabetes1();

/**
* @brief gets fat table 2
*
* @return pointer to fat table 2
*/
uint16_t* getDiabetes2();

/**
* @brief gets current working directory BYTE*
*
* @return pointer to current working directory
*/
BYTE* getCWD();

BYTE* getRoot();

char* getCwdPath();

/**
* @brief gets root directory
*
* @return pointer to root directory
*/
char* getImagePath();

/**
* @brief sets cwd path
*
* @param jerry string to make cwdPath
*/
void setCwdPath(const char* jerry);

// Depricated, do not use
// void trim (char *dest, char *src);