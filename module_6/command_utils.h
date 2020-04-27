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

/**
* @brief A struct to hold an directory entry
*
* @param empty If the entry is empty
* @param fileName The name of the entry
* @param extension The extension, type, of the entry
* @param attributes The attributes of the entry
* @param reserved If the entry is resereved
* @param creationTime Entry creation time
* @param creationHour Entry creation hour
* @param creationMin Entry creation min
* @param creationSec Entry creation sec
* @param creationDate Entry creation date
* @param creationYear Entry creation year
* @param creationMonth Entry creation month
* @param creationDay Entry creation day
* @param lastAccessDate Entry access date
* @param lastAccessYear Entry access year
* @param lastAccessMonth Entry access month
* @param lastAccessDay Entry access day
* @param lastWriteTime Entry last write time
* @param lastWriteHour Entry last write hour
* @param lastWriteMin Entry last write min
* @param lastWriteSec Entry last write sec
* @param lastWriteDate Entry last write date
* @param lastWriteYear Entry last write year
* @param lastWriteMonth Entry last write month
* @param lastWriteDay Entry last write day
* @param firstLogicalCluster First logical cluster
* @param fileSize Entry filesize
 */
typedef struct ENTRY {
	BYTE empty;
	char fileName[9];
	char extension[4];
	BYTE attributes;
	uint16_t reserved;
	uint16_t creationTime;
	uint8_t creationHour;
	uint8_t creationMin;
	uint8_t creationSec;
	uint16_t creationDate;
	uint16_t creationYear;
	uint8_t creationMonth;
	uint8_t creationDay;
	uint16_t lastAccessDate;
	uint16_t lastAccessYear;
	uint8_t lastAccessMonth;
	uint8_t lastAccessDay;
	uint16_t lastWriteTime;
	uint8_t lastWriteHour;
	uint8_t lastWriteMin;
	uint8_t lastWriteSec;
	uint16_t lastWriteDate;
	uint16_t lastWriteYear;
	uint8_t lastWriteMonth;
	uint8_t lastWriteDay;
	uint16_t firstLogicalCluster;
	uint32_t fileSize;
} ENTRY;

// TODO: Define this struct
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

/**
* @brief A memcpy function that takes all characters to their uppercase equivalent
* 
* @param dest The destination char pointer
* @param source The source char pointer
* @param nchars The number of chars to copy, and toUpper, from source to dest
*
* @return void method, copies in place
*/
void memcpyUpper(char*,const char*, int);

// Do not include
int strcimp(char const *a, char const* b);

/**
* @brief Star search for ls
*
* @param starryboi The char pointer that contians a star search
*/
uint32_t starsearch(const char* starryboi, const char* fileboi);
/*
* @brief Finds the dot position of a certain string
*
* @returns dot position uint16_t
*/
uint16_t findDotPosition(const char *);