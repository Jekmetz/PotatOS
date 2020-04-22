
#include <stdio.h>
#include <malloc.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include <ctype.h>









uint32_t copy(uint32_t startingLoc, uint32_t numBytes, unsigned char bs[SECTORSIZE]);
char hexToAscii( char first, char second);
char*  charToBin(char c);
int pwd(ENTRY *cwdIn);
int type(unsigned char *whole, uint16_t *FAT, ENTRY *cwdIn, char *fileName);

uint32_t loadRootDir(FILE *fpIn, ENTRY *rootDirIn);
uint32_t printRootDir(ENTRY *rootDirIn);

uint32_t main() {

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

int type(BYTE *whole, uint16_t *FAT, ENTRY *cwdIn, char *fileName){
	
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

