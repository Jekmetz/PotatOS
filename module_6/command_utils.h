#pragma once

#include <stdint.h>

// Sector size is 512 bytes always with FAT12
#define SECTORSIZE 512
#define SECTORSPERFAT 9 
#define FATTABLESIZE ((SECTORSPERFAT * SECTORSIZE * 8) / 12)
#define MAXDIRENTRY 16
#define MAXENTRIESPERDIR 16

#define DEBLOC() printf("\033[33;1m%s(%d)\033[0m\n",__FILE__,__LINE__)

typedef unsigned char BYTE;

typedef int (*CommandFunction)(int, char **);

struct command {
    char *command_name;
    CommandFunction function;
};

typedef struct ENTRY {
	BYTE empty;
	char fileName[9];
	char extension[4];
	BYTE attributes;
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
	char *dirName;
	uint32_t sectorStart;
} PREVDIR;

typedef struct command command_t;

char *trim_whitespace(char *str);

uint32_t split_args(char *command, char **argv);
