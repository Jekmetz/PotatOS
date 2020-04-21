
#include <stdio.h>
#include <malloc.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>

// Sector size is 512 bytes always with FAT12
#define SECTORSIZE 512
#define SECTORSPERFAT 8 
#define FATTABLESIZE ((SECTORSPERFAT * SECTORSIZE * 8) / 12)
#define MAXDIRENTRY 16
#define MAXENTRIESPERDIR 16

typedef unsigned char BYTE;

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
	unsigned char *volumeLabel;
	unsigned char *fileSystemType;
} BOOTSECTORSTRUCT;

typedef struct ENTRY {
	BYTE empty;
	unsigned char *fileName;
	unsigned char *extension;
	uint16_t attributes;
	uint16_t reserved;
	uint16_t creationTime;
	uint16_t creationDate;
	uint16_t lastAccessDate;
	uint16_t lastWriteTime;
	uint16_t lastWriteDate;
	uint16_t firstLogicalCluster;
	uint32_t fileSize;
} ENTRY;

typedef struct PREVDIR {
	unsigned char *dirName;
	int sectorStart;
} PREVDIR;

uint32_t loadBootSector(FILE *fpIn, struct BOOTSECTORSTRUCT *bootSectorIn);
uint32_t printBootSector(struct BOOTSECTORSTRUCT *bootSectorIn);
uint32_t copy(uint32_t startingLoc, uint32_t numBytes, unsigned char bs[SECTORSIZE]);
char hexToAscii( char first, char second);
uint16_t *loadFAT(BYTE *fpIn, uint32_t startingSector);
char*  charToBin(char c);
int pwd(ENTRY *cwdIn);
int type(unsigned char *whole, int *FAT, ENTRY *cwdIn, char *fileName);
void trim (char *dest, char *src);
uint32_t loadRootDir(FILE *fpIn, ENTRY *rootDirIn);
uint32_t printRootDir(ENTRY *rootDirIn);
ENTRY *loadCWD(FILE *fpIn, uint32_t startingSec);

uint32_t main() {
	FILE *fp;
	char* filename = "/home/jekmetz/Downloads/cs450_2.img"; 

	fp = fopen(filename, "r");
	if(fp == NULL){
		printf("Did not load\n");
		return 0;
	}

	fseek(fp, 0, SEEK_END);
	uint32_t size = ftell(fp);
	fseek(fp, 0, SEEK_SET);

	// Size of entire system
	BYTE *ENTIRESYSTEM = malloc(sizeof(char) * size);

	// Reading the ENTIRE DAMN THING
	fread(ENTIRESYSTEM, size, 1, fp);


	// Boot sector does not need to be an array, it is only one struct

	BOOTSECTORSTRUCT *bootSector = malloc(sizeof(BOOTSECTORSTRUCT));
	
	// Loading boot sector
	loadBootSector(fp, bootSector);

	// uint32_t array pointer to hold FATs
	uint16_t *fat1;
	uint16_t *fat2;

	// The starting sectors of the two FATs
	uint32_t fat1StartingSec = 1;
	uint32_t fat2StartingSec = 10;

	// Loading both FATs into arrays
	fat1 = loadFAT(ENTIRESYSTEM, fat1StartingSec);
	fat2 = loadFAT(ENTIRESYSTEM, fat2StartingSec);

	ENTRY *cwd;
	uint32_t rootDirStartingSec = 19;

	cwd = loadCWD(ENTIRESYSTEM, rootDirStartingSec);

	printf("%s\tFile size: %d\tFirst logical cluster: %d\n", cwd[3].fileName,cwd[3].fileSize, cwd[3].firstLogicalCluster);

//NICKS COMMAND HANDLER
	int exit = 0;

	char cmd[50];

	while(exit != 1){
		fgets(cmd, 50, stdin);
		// Removing newline
		cmd[strcspn(cmd, "\n")] = '\0';

		if(strcmp(cmd, "exit") == 0){
			exit = 1;
		}
		else if(strcmp(cmd, "ls") == 0){
			pwd(cwd);
		}
		else if(strncmp(cmd, "cd", strlen("cd")) == 0){
			// Grabbing first token which should be "cd"
			char *dir = strtok(cmd, " ");

			// Attempting to grab second
			dir = strtok(NULL, " ");

			if(dir != NULL){
				// Stripping off newline characters
				// dir[strcspn(dir, "\n")] = '\0';

				for(int i = 0; i<MAXENTRIESPERDIR; i++){
					// if(cwd[i].empty != 1){
					// 	printf("%s\n", cwd[i].fileName);
					// }
					if(cwd[i].empty != 1 && strcmp(cwd[i].fileName, dir) == 0){
						printf("Yep: %s\tGo to raw: %d\n", cwd[i].fileName, cwd[i].firstLogicalCluster);
						cwd = loadCWD(ENTIRESYSTEM, 33 + cwd[i].firstLogicalCluster - 2);
					}
				}
			}
			else{
				printf("Have to specify dir to go to next\n");
			}

		}
		else if(strncmp(cmd, "type", strlen("type")) == 0){
			// Grabbing first token which should be "cd"
			char *dir = strtok(cmd, " ");

			// Attempting to grab second
			dir = strtok(NULL, " ");

			if(dir != NULL){
				// Stripping off newline characters
				// dir[strcspn(dir, "\n")] = '\0';

				type(ENTIRESYSTEM, fat1, cwd, dir);
			}
			else{
				printf("Have to specify what file you would like to print\n");
			}
		}
		else if(strcmp(cmd, "root") == 0){
			cwd = loadCWD(ENTIRESYSTEM, rootDirStartingSec);
		}
		else if(strcmp(cmd, "printBoot") == 0){
			printBootSector(bootSector);
		}
		else{
			printf("Unknown command\n");
		}
	}
}

int type(unsigned char *whole, int *FAT, ENTRY *cwdIn, char *fileName){
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
			printf("Filename: %s\tFirst logical cluster: %d\tSize: %d\tNeeded jumps: %d\n", \
				cwdIn[i].fileName, cwdIn[i].firstLogicalCluster, cwdIn[i].fileSize, cwdIn[i].fileSize / 512);


			int current;
			current = cwdIn[i].firstLogicalCluster;

			BYTE *out;

			out = (BYTE*) malloc(sizeof(BYTE) * cwdIn[i].fileSize);

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
}

int pwd(ENTRY *cwdIn){
	char *prefix = malloc(sizeof(cwdIn[0].fileName));
	strcpy(prefix, cwdIn[0].fileName);

	for(int i = 0; i< MAXENTRIESPERDIR; i++){
		if(cwdIn[i].attributes != 0x28 && cwdIn[i].empty != 1){
			printf("User@potatOS:/%s/%s.%s\n", prefix, cwdIn[i].fileName,cwdIn[i].extension);
		}
	}
}

int *loadCWD(unsigned char *whole, int startingSec){
	// ENTRY array to the cws 
	ENTRY *cwd = malloc(sizeof(ENTRY) * MAXENTRIESPERDIR);

	for(uint32_t i = 0; i<MAXENTRIESPERDIR; i++){

		// printf("%d\t%02X\n",i,  *(whole + (i * 32)));

		unsigned char *fileName = malloc(sizeof(char) * 8);
		unsigned char *extension = malloc(sizeof(char) * 3);


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
				
				cwd[i].attributes = 			*((char*)  (temp + (32 * i) + 11)); 
				cwd[i].reserved = 				*((uint16_t*) (temp + (32 * i) + 12));
				cwd[i].creationTime = 			*((uint16_t*) (temp + (32 * i) + 14));
				cwd[i].creationDate = 			*((uint16_t*) (temp + (32 * i) + 16));
				cwd[i].lastAccessDate = 		*((uint16_t*) (temp + (32 * i) + 18));
				cwd[i].lastWriteTime = 			*((uint16_t*) (temp + (32 * i) + 22));
				cwd[i].lastWriteDate = 			*((uint16_t*) (temp + (32 * i) + 24));
				cwd[i].firstLogicalCluster = 	*((uint16_t*) (temp + (32 * i) + 26));
				cwd[i].fileSize = 				*((uint32_t*)   (temp + (32 * i) + 28));
		}
	}

	return cwd;
}

uint32_t loadBootSector(FILE *fpIn, BOOTSECTORSTRUCT *bootSectorIn){
	// Loading Bytes Per Sector
	// Starting location 11 bytes

	fseek(fpIn, 0, SEEK_SET);

	unsigned char *volumeLabel = malloc(sizeof(char) * 11);
	unsigned char *fileSystemType = malloc(sizeof(char) * 8);

	unsigned char bs[SECTORSIZE];
	
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
	
	return 1;
}

uint32_t printBootSector(BOOTSECTORSTRUCT *bootSectorIn){
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
}

uint16_t *loadFAT(BYTE* sys, uint32_t startingSector) {	
	
	//move sys forward to the correct starting position
	sys = (sys + SECTORSIZE * startingSector);

	//fat table malloc
	uint16_t *fatTable = malloc(sizeof(uint16_t) * FATTABLESIZE);

	// For loop to iterate over FAT to insert 12 bit long FAT entry
	BYTE* si;
	uint16_t boi0,boi1;
	for(uint32_t i = 0; i<  FATTABLESIZE/2;i++){
		
		si = sys + i*3;
		//make our phatty bois
		boi0,boi1;

		boi0 = ((*(si+1)&0x0F)<<8) + (*(si));
		boi1 = (*(si+2)<<4) + (*(si+1)>>4);

		// printf("---------------\n*(sys+si): %x\n,*(sys+si+1): %x\n,*(sys+si+2): %x\n(*(sys+si+1)|0x0F)<<8: %x\n*(sys+si): %x\nboi0: %x\nboi1: %x\n*(sys+si+2)<<4: %x\n(*(sys+si+1)>>4): %x\n"
		// 	,*(sys+si),*(sys+si+1),*(sys+si+2),(*(sys+si+1)&0x0F)<<8,*(sys+si),boi0,boi1,*(sys+si+2)<<4,(*(sys+si+1)>>4));
		//voila
		
		fatTable[2*i] = boi0;
		fatTable[2*i+1] = boi1;
	}

	return fatTable;
}

uint32_t loadRootDir(FILE *fpIn, ENTRY *rootDirIn) {
	// Defining the root starting sector
	uint32_t rootDirStartingSector = 19;
	
	// Setting the file to the correct location
	fseek(fpIn, SECTORSIZE*rootDirStartingSector, SEEK_SET);

	// Overall rootDir char array to hold entire sector
	unsigned char rootDir[SECTORSIZE];

	// Reading file into the rootDir array
	fread(rootDir, SECTORSIZE, 1, fpIn);

	uint32_t entry = 0;

	while(entry < 16 && rootDir[entry*32] != 0x00){
		unsigned char *fileName = malloc(sizeof(char) * 8);
		unsigned char *extension = malloc(sizeof(char) * 3);

		memcpy(fileName, rootDir+0, sizeof(char) * 8);
		rootDirIn->fileName = fileName;
		memcpy(extension, rootDir+8, sizeof(char) * 3);
		rootDirIn->extension = extension;
		rootDirIn->attributes = *((char*)(rootDir+11)); 
		rootDirIn->reserved = *((uint16_t*)(rootDir+12));
		rootDirIn->creationTime = *((uint16_t*)(rootDir+14));
		rootDirIn->creationDate = *((uint16_t*)(rootDir+16));
		rootDirIn->lastAccessDate = *((uint16_t*)(rootDir+18));
		rootDirIn->lastWriteTime = *((uint16_t*)(rootDir+22));
		rootDirIn->lastWriteDate = *((uint16_t*)(rootDir+24));
		rootDirIn->firstLogicalCluster = *((uint16_t*)(rootDir+26));
		rootDirIn->fileSize = *((uint32_t*)(rootDir+28));
	}
	return 1;
}

uint32_t printRootDir(ENTRY *rootDirIn){
	printf("File name: %s\n", rootDirIn->fileName);
	printf("Extension: %s\n", rootDirIn->extension);
	printf("Attributes: %d\n", rootDirIn->attributes);
	printf("Reserved: %d\n", rootDirIn->attributes);
	printf("Creation Time: %d\n", rootDirIn->creationTime);
	printf("Creation Date: %d\n", rootDirIn->creationDate);
	printf("Last Access Date: %d\n", rootDirIn->lastAccessDate);
	printf("Last Write Time: %d\n", rootDirIn->lastWriteTime);
	printf("Last Write Date: %d\n",rootDirIn->lastWriteDate);
	printf("First Logical Cluster: %d\n", rootDirIn->firstLogicalCluster);
	printf("File Size: %d\n", rootDirIn->fileSize);
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