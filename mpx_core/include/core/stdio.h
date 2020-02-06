/**
* @file stdio.h
*
* @brief Holds all prototypes of standard I/O functions
*/
#ifndef STD_IO_H_
#define STD_IO_H_

/**
* @brief takes in a format string and prints it out to the DEFAULT_DEVICE
*
* %c - charachter
* %d/%i - decimal integer
* %x - hexadecimal integer
* %s - string
* %% - percent sign
* Numbers can be included before the format specifier to declare
* alignment. i.e. %-10c = "c         "
* A pad with 0s can also be added using a '0' directly after the percent
* i.e. %03c = "00c"
*
* @param form character pointer to the format
* @param valist variadic arguments to match the format (see brief)
*
* @return 0 for failure 1 for success
*/
int printf(char *form, ...);

int puts(char *buffer);
#endif