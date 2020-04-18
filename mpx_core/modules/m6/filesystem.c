
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

int loadBootSector(FILE *fpIn, struct BOOTSECTORSTRUCT *bootSectorIn);
int printBootSector(struct BOOTSECTORSTRUCT *bootSectorIn);
int copy(int startingLoc, int numBytes, unsigned char bs[SECTORSIZE]);
char hexToAscii( char first, char second);
int *loadFAT(FILE *fpIn, int startingSector);
char*  charToBin(char c);
int stringToDec(char *in);
int loadRootDir(FILE *fpIn, ENTRY *rootDirIn);
int printRootDir(ENTRY *rootDirIn);
int *loadCWD(FILE *fpIn, int startingSec);

int main() {
	FILE *fp;
	char* filename = "/home/nick/Downloads/cs450_2.img"; 

	fp = fopen(filename, "r");
	if(fp == NULL){
		printf("Did not load\n");
	}

	struct stat st;
	fstat(fp, &st);
	int size = st.st_size

	// Size of entire system
	unsigned char *ENTIRESYSTEM = malloc(sizeof(char) * size);

	// Reading the ENTIRE DAMN THING
	fread(ENTIRESYSTEM, size, 1, fpIn);


	// Boot sector does not need to be an array, it is only one struct
	BOOTSECTORSTRUCT *bootSector = malloc(sizeof(BOOTSECTORSTRUCT));
	
	// Loading boot sector
	loadBootSector(fp, bootSector);

	// Int array pointer to hold FATs
	int *fat1;
	int *fat2;

	// The starting sectors of the two FATs
	int fat1StartingSec = 1;
	int fat2StartingSec = 10;

	// Loading both FATs into arrays
	fat1 = loadFAT(fp, fat1StartingSec);
	fat2 = loadFAT(fp, fat2StartingSec);

	// TODO: Implement check to make sure that FATs are the same?

	ENTRY *cwd;
	int rootDirStartingSec = 19;

	cwd = loadCWD(fp, rootDirStartingSec);


	for(int i = 0; i< MAXENTRIESPERDIR; i++){
		// printf("%d\n", cwd[i].empty);
		if(cwd[i].empty != 1){
			printf("Name: %s.%s\n\tType: %02X\n", cwd[i].fileName,cwd[i].extension, cwd[i].attributes);
		}
	}

	// ENTRY *rootDir = malloc(sizeof(ENTRY));

	// ENTRY *rootDir = malloc(sizeof(ENTRY));

	// loadRootDir(fp, rootDir);

	// Prints below
	// printBootSector(bootSector);
	// // printRootDir(rootDir);

	// for(int i = 0; i< FATTABLESIZE; i++){
	// 	printf("%d\t%x\t%x\n",i , fat1[i], fat2[i]);
	// }
}

int *loadCWD(FILE *fpIn, int startingSec){
	// Temp array to hold raw data from file pointer
	unsigned char temp[SECTORSIZE];
	
	// Setting file at first FAT
	fseek(fpIn, (SECTORSIZE * startingSec), SEEK_SET);

	// // Reading into temp array
	fread(temp, SECTORSIZE, 1, fpIn);

	// ENTRY array to the cws 
	ENTRY *cwd = malloc(sizeof(ENTRY) * MAXENTRIESPERDIR);

	for(int i = 0; i<MAXENTRIESPERDIR; i++){

		// printf("%d\t%02X\n",i,  *(temp + (i * 32)));

		unsigned char *fileName = malloc(sizeof(char) * 8);
		unsigned char *extension = malloc(sizeof(char) * 3);


		// Index is deleted
		if ( *(temp + (i * 32)) == 0xE5) {
			cwd[i].empty = 1;
		}
		// Last index
		else if ( *(temp + (i * 32)) == 0x00){
			cwd[i].empty = 1;
			// TODO: Stop 
		}
		else{
				cwd[i].empty = 0;
				memcpy(fileName, temp + (32 * i) + 0, sizeof(char) * 8);
				cwd[i].fileName = fileName;
				memcpy(extension, temp + (32 * i) + 8, sizeof(char) * 3);
				cwd[i].extension = extension;
				
				cwd[i].attributes = 			*((char*)  (temp + (32 * i) + 11)); 
				cwd[i].reserved = 				*((short*) (temp + (32 * i) + 12));
				cwd[i].creationTime = 			*((short*) (temp + (32 * i) + 14));
				cwd[i].creationDate = 			*((short*) (temp + (32 * i) + 16));
				cwd[i].lastAccessDate = 		*((short*) (temp + (32 * i) + 18));
				cwd[i].lastWriteTime = 			*((short*) (temp + (32 * i) + 22));
				cwd[i].lastWriteDate = 			*((short*) (temp + (32 * i) + 24));
				cwd[i].firstLogicalCluster = 	*((short*) (temp + (32 * i) + 26));
				cwd[i].fileSize = 				*((int*)   (temp + (32 * i) + 28));
		}
	}

	/*
	Notes to self:
		* Currently working in this function. Need to get it so that main only prints out used entries
		* Need to figure out DOS date and time
		* Need to figure out hidden attribute and how to use it
	*/

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