/********************************************************
* This file will hold utility functions for this project
*********************************************************/
#include <string.h>

/**
* @brief Determines if a passed character is a null or space
*
* @param test charachter to test
*
* @return 1 if space or null, 0 otherwise
*/
int isnullorspace(char test) {
  return (isspace(&test) || test == '\0');
}