
#include <stdio.h>
#include <malloc.h>
#include <string.h>
#include <stdlib.h>
#include <sys/stat.h>  

// Sector size is 512 bytes always with FAT12
#define SECTORSIZE 512
#define SECTORSPERFAT 8 
#define FATTABLESIZE ((SECTORSPERFAT * SECTORSIZE * 8) / 12)
#define MAXDIRENTRY 16
#define MAXENTRIESPERDIR 16


typedef struct BOOTSECTORSTRUCT {
	int bytesPerSector;
	int sectorsPerCluster;
	int numReservedSectors;
	int numFATCopies;
	int maxNumRoot;
	int numOfSectors;
	int numSECTORSPERFAT;
	int sectorsPerTrack;
	int numberHeads;
	int sectorCountFAT32;
	int bootSig;
	int volumeID;
	unsigned char *volumeLabel;
	unsigned char *fileSystemType;
} BOOTSECTORSTRUCT;

typedef struct ENTRY {
	int empty;
	unsigned char *fileName;
	unsigned char *extension;
	int attributes;
	int reserved;
	int creationTime;
	int creationDate;
	int lastAccessDate;
	int lastWriteTime;
	int lastWriteDate;
	int firstLogicalCluster;
	int fileSize;
} ENTRY;

typedef struct PREVDIR {
	unsigned char *dirName;
	int sectorStart;
} PREVDIR;

int loadBootSector(FILE *fpIn, struct BOOTSECTORSTRUCT *bootSectorIn);
int printBootSector(struct BOOTSECTORSTRUCT *bootSectorIn);
int copy(int startingLoc, int numBytes, unsigned char bs[SECTORSIZE]);
char hexToAscii( char first, char second);
int *loadFAT(FILE *fpIn, int startingSector);
char*  charToBin(char c);
int stringToDec(char *in);
int loadRootDir(FILE *fpIn, ENTRY *rootDirIn);
int printRootDir(ENTRY *rootDirIn);
int *loadCWD(unsigned char *whole, int startingSec);
void trim (char *dest, char *src);
int pwd(ENTRY *cwdIn);
int type(unsigned char *whole, int *FAT, ENTRY *cwdIn, char *fileName);

int main() {
	FILE *fp;
	char* filename = "/home/nick/Downloads/cs450_2.img"; 

	fp = fopen(filename, "r");
	if(fp == NULL){
		printf("Did not load\n");
	}

	// // Boot sector does not need to be an array, it is only one struct
	BOOTSECTORSTRUCT *bootSector = malloc(sizeof(BOOTSECTORSTRUCT));
	
	// Loading boot sector
	loadBootSector(fp, bootSector);

	
	fseek(fp, 0L, SEEK_END);
	int size = ftell(fp);
	fseek(fp, 0, SEEK_SET);

	// Size of entire system
	unsigned char *ENTIRESYSTEM = malloc(size);

	// Reading the ENTIRE DAMN THING
	fread(ENTIRESYSTEM, size, 1, fp);
	
	// Int array pointer to hold FATs
	int *fat1;
	int *fat2;

	// The starting sectors of the two FATs
	int fat1StartingSec = 1;
	int fat2StartingSec = 10;

	// Loading both FATs into arrays
	fat1 = loadFAT(fp, fat1StartingSec);
	fat2 = loadFAT(fp, fat2StartingSec);

	ENTRY *cwd;
	int rootDirStartingSec = 19;

	cwd = loadCWD(ENTIRESYSTEM, rootDirStartingSec);

	printf("%s\tFile size: %d\tFirst logical cluster: %d\n", cwd[3].fileName,cwd[3].fileSize, cwd[3].firstLogicalCluster);

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

			char *out;

			// out = malloc(sizeof(char) * (SECTORSIZE * (cwdIn[i].fileSize / 512)));
			out = malloc(sizeof(char) * 598367 * 2);

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

	for(int i = 0; i<MAXENTRIESPERDIR; i++){

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
				
				cwd[i].attributes = 			*((char*)  (whole + (startingSec * SECTORSIZE) + (32 * i) + 11)); 
				cwd[i].reserved = 				*((short*) (whole + (startingSec * SECTORSIZE) + (32 * i) + 12));
				cwd[i].creationTime = 			*((short*) (whole + (startingSec * SECTORSIZE) + (32 * i) + 14));
				cwd[i].creationDate = 			*((short*) (whole + (startingSec * SECTORSIZE) + (32 * i) + 16));
				cwd[i].lastAccessDate = 		*((short*) (whole + (startingSec * SECTORSIZE) + (32 * i) + 18));
				cwd[i].lastWriteTime = 			*((short*) (whole + (startingSec * SECTORSIZE) + (32 * i) + 22));
				cwd[i].lastWriteDate = 			*((short*) (whole + (startingSec * SECTORSIZE) + (32 * i) + 24));
				cwd[i].firstLogicalCluster = 	*((short*) (whole + (startingSec * SECTORSIZE) + (32 * i) + 26));
				cwd[i].fileSize = 				*((int*)   (whole + (startingSec * SECTORSIZE) + (32 * i) + 28));
		}
	}

	return cwd;
}

int loadBootSector(FILE *fpIn, BOOTSECTORSTRUCT *bootSectorIn){
	// Loading Bytes Per Sector
	// Starting location 11 bytes

	fseek(fpIn, 0, SEEK_SET);

	unsigned char *volumeLabel = malloc(sizeof(char) * 11);
	unsigned char *fileSystemType = malloc(sizeof(char) * 8);

	unsigned char bs[SECTORSIZE];
	
	fread(bs, SECTORSIZE, 1, fpIn);

	bootSectorIn->bytesPerSector = *((short*)(bs+11));
	bootSectorIn->sectorsPerCluster = *((char*)(bs+13));
	bootSectorIn->numReservedSectors = *((short*)(bs+14));
	bootSectorIn->numFATCopies = *((char*)(bs+16));
	bootSectorIn->maxNumRoot = *((short*)(bs+17));
	bootSectorIn->numOfSectors = *((short*)(bs+19));
	bootSectorIn->numSECTORSPERFAT = *((short*)(bs+22));
	bootSectorIn->sectorsPerTrack = *((short*)(bs+24));
	bootSectorIn->numberHeads = *((short*)(bs+26));
	bootSectorIn->sectorCountFAT32 = *((int*)(bs+32));
	bootSectorIn->bootSig = *((char*)(bs+38));
	bootSectorIn->volumeID = *((int*)(bs+32));
	memcpy(volumeLabel, bs+43, sizeof(char) * 11);
	bootSectorIn->volumeLabel = volumeLabel;
	memcpy(fileSystemType, bs+54, sizeof(char) * 8);
	bootSectorIn->fileSystemType = fileSystemType;
	
	return 1;
}

int printBootSector(BOOTSECTORSTRUCT *bootSectorIn){
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

int *loadFAT(FILE *fpIn, int startingSector) {
	// Temp array to hold the data before we swith it around
	unsigned char temp[SECTORSIZE * SECTORSPERFAT];
	
	// Array to hold each bytes binary representation
	unsigned char *binaryArray[SECTORSIZE * SECTORSPERFAT];

	// Setting file at first FAT
	fseek(fpIn, (SECTORSIZE * startingSector), SEEK_SET);

	// Reading into temp array
	fread(temp, SECTORSIZE * SECTORSPERFAT, 1, fpIn);
	
	// Int array to hold FAT 
	int *fatTable = malloc(sizeof(int) * FATTABLESIZE);

	// Char pointer to hold binary representation
	char *tempmes;

	// For loop that iterates over temp and copies bytes into binary values into binaryArray
	for(int i = 0; i< SECTORSIZE * SECTORSPERFAT; i++){
		// Calling charToBin to get binary representation
		tempmes = charToBin(temp[i]);
		
		//Assigning into array 
		binaryArray[i] = tempmes;
	}

	// For loop to iterate over FAT to insert 12 bit long FAT entry
	for(int i = 0; i<  FATTABLESIZE;i++){
		// Odd
		if(i % 2){
			/*
			In odd FAT Entries
			We pull the 8 bits from index
				((3*i)/2) + 1
			And the starting four bits from index
				((1+(3*i))/2) - 1
			*/
			// The indicies of the two locations that we will need to pull data from, named after their portions that they will pull from
			int eightbitsIndex = (((3*i)/2)+1);
			int startingFourBitsIndex = (((1+(3*i))/2)-1);

			// Char pointers to hold the pulling of info from the array of raw data
			char *eight = malloc(sizeof(char) * 8);
			char *startingFour = malloc(sizeof(char) * 4);

			// Copying the raw data from the array into the char pointers
			strncpy(eight, binaryArray[eightbitsIndex], 8);
			strncpy(startingFour, binaryArray[startingFourBitsIndex], 4);

			// Char pointer to hold the final string of data 
			char *final = malloc(sizeof(char) * 12);
			// Copying the first portion into final
			strncpy(final, eight, 8);
			// Concatinating the final portion on the final string
			strcat(final, startingFour);

			// Sending to stringToDec function to get integer from string of binary
			int finalInt = stringToDec(final);

			// Print to show, will be removed later
			// printf("%d\t%s\t%x\n",i, final, finalInt);

			// Putting the finalInt into the array 
			fatTable[i] = finalInt;
		}

		// Even
		else{
			/*
			In even FAT Entries
			We pull the last four bits from index
				((1+(3*i))/2) + 1
			And the 8 bits from index
				(3*i)/2
			*/
			// The indicies of the two locations that we will need to pull data from, named after their portions that they will pull from
			int endingFourBits = (((1+(3*i))/2) + 1);
			int eightbitsIndex = (((3*i))/2);
			
			// Char pointers to hold the pulling of info from the array of raw data
			char *endingFour = malloc(sizeof(char) *4);
			char *eight = malloc(sizeof(char) * 8);

			// Copying the raw data from the array into the char pointers
			strncpy(endingFour, binaryArray[endingFourBits] + 4, 4);
			strncpy(eight, binaryArray[eightbitsIndex], 8);

			// Char pointer to hold the final string of data
			char *final = malloc(sizeof(char) * 12);
			// Copying the first portion into final
			strncpy(final, endingFour, 4);
			// Concatinating the final portion on the final string
			strcat(final, eight);

			// Sending to stringToDec function to get integer from string of binary
			int finalInt = stringToDec(final);

			// Print to show the finalInt, will be removed later
			// printf("%d\t%s\t%x\n", i, final, finalInt);
			
			// Putting the finalInt into the array
			fatTable[i] = finalInt;
		}
	}

	return fatTable;
}

int loadRootDir(FILE *fpIn, ENTRY *rootDirIn) {
	// Defining the root starting sector
	int rootDirStartingSector = 19;
	
	// Setting the file to the correct location
	fseek(fpIn, SECTORSIZE*rootDirStartingSector, SEEK_SET);

	// Overall rootDir char array to hold entire sector
	unsigned char rootDir[SECTORSIZE];

	// Reading file into the rootDir array
	fread(rootDir, SECTORSIZE, 1, fpIn);

	int entry = 0;

	while(entry < 16 && rootDir[entry*32] != 0x00){
		unsigned char *fileName = malloc(sizeof(char) * 8);
		unsigned char *extension = malloc(sizeof(char) * 3);

		memcpy(fileName, rootDir+0, sizeof(char) * 8);
		rootDirIn->fileName = fileName;
		memcpy(extension, rootDir+8, sizeof(char) * 3);
		rootDirIn->extension = extension;
		rootDirIn->attributes = *((char*)(rootDir+11)); 
		rootDirIn->reserved = *((short*)(rootDir+12));
		rootDirIn->creationTime = *((short*)(rootDir+14));
		rootDirIn->creationDate = *((short*)(rootDir+16));
		rootDirIn->lastAccessDate = *((short*)(rootDir+18));
		rootDirIn->lastWriteTime = *((short*)(rootDir+22));
		rootDirIn->lastWriteDate = *((short*)(rootDir+24));
		rootDirIn->firstLogicalCluster = *((short*)(rootDir+26));
		rootDirIn->fileSize = *((int*)(rootDir+28));
	}
	return 1;
}

int printRootDir(ENTRY *rootDirIn){
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

char* charToBin(char c) {
	char *temp;
	temp = malloc(sizeof(char) * 7);
	int count = 0;
    for (int i = 7; i >= 0; --i)
    {
        temp[count] = ( (c & (1 << i)) ? '1' : '0' );
    	count ++;
    }
  	
  	// printf("%s\n", temp);
  	return temp;
}

int stringToDec(char *in){
  unsigned char *p = in;
  unsigned int   r = 0;

  while (p && *p ) {
    r <<= 1;
    r += (unsigned int)((*p++) & 0x01);
  }

  return (int)r;
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