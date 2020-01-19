/********************************************************
* This file will hold utility functions for this project
*********************************************************/
#include <string.h>

/*
        Procedure: isnullorspace
        Description: returns true if test is '\0' or a space
        Parameters: test-character to test
*/
int isnullorspace(char test) {
  return (isspace(&test) || test == '\0');
}