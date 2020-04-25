/**
* @file command_utils.h
*
* @brief Holds standard typedefs and utility function headers for certain commands
*/
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

/**
 * @brief A struct to hold command information
 *
 * @param command_name The command's name
 * @param function A pointer to the command's function
 * @param command_usage Syntax for executing the command
 * @param command_description A description of the command
 * @param command_examples Example uses of the command
 */
struct command {
    char *command_name;
    CommandFunction function;
    char *command_usage;
    char *command_description;
    char *command_examples;
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

/**
 * @brief A function to trim strings
 *
 * @param str The string to trim
 *
 * @return A pointer to the beginning of the trimmed string
 */
char *trim_whitespace(char *str);

/**
 * @brief A function to split up a input string into arguments for command functions
 *
 * @param The command input to split up
 * @param A pointer to the destination of the argument slices, argv
 *
 * @return The amount of arguments split up, argc
 */
uint32_t split_args(char *command, char **argv);

void memcpyUpper(char*,const char*, int);

int strcimp(char const *a, char const* b);

uint16_t findDotPosition(const char *);