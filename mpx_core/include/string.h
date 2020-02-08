/**
* @file string.h
*
* @brief Holds all utility prototypes used to modify strings
*/
#pragma once

#ifndef _STRING_H
#define _STRING_H

#include <system.h>
#include <stdarg.h>

/*
  Procedure..: isspace
  Description..: Determine if a character is whitespace.
  Params..: c-character to check
*/
int isspace(const char* c);

/*
  Procedure..: memset
  Description..: Set a region of memory.
  Params..: s-destination, c-byte to write, n-count
*/
void* memset(void* s, int c, size_t n);

/*
  Procedure..: strcpy
  Description..: Copy one string to another.
  Params..: s1-destination, s2-source
*/
char* strcpy(char* s1, const char* s2);

/*
  Procedure..: strcat
  Description..: Concatenate the contents of one string onto another.
  Params..: s1-destination, s2-source
*/
char* strcat(char* s1, const char* s2);

/*
  Procedure..: strlen
  Description..: Returns the length of a string.
  Params..: s-input string
*/
int strlen(const char* s);

/*
  Procedure..: strcmp
  Description..: String comparison
  Params..: s1-string 1, s2-string 2
*/
int strcmp(const char* s1, const char* s2);

/*
  Procedure..: strtok
  Description..: Split string into tokens
  Params..: s1-string, s2-delimiter
*/
char* strtok(char* s1, const char* s2);

/**
* @brief Checks if char c is a digit
*
* @param c character to check
*
* @return is digit: 1; is not digit: 0;
*/
int isdigit(char c);

/**
* @brief reverse a string from 0 to j
*
* @param str string to reverse
* @param j index to reverse str to
*
* @return pointer to str
*/
char* reverse(char* str, int j);

/*
  Procedure..: atoi
  Description..: Convert an ASCII string to an integer
  Params..: const char *s -- String
*/
int atoi(const char* s);

/**
* @brief Converts integer to string
*
* @param num number to convert
* @param str string to store result in
* @param base base to convert to
*
* @return pointer to str
*/
char* itoa(int num, char* str, int base);

/**
* @brief Visible representation of the sprintf function
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
* @param buffer character pointer to store spaces to
* @param format format string with format specifiers
* @param valist variadic list with parameters matching the format
*
* @return pointer to buffer
*/
int sprintf(char* buffer, char* format, ...);

/**
* @brief Main implementation of the sprintf function
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
* @param buffer character pointer to store spaces to
* @param format format string with format specifiers
* @param valist variadic list with parameters matching the format
*
* @return pointer to buffer
*/
int sprintf_internal(char *buffer, char *format, va_list valist);

/**
* @brief Returns the lowercase representation of a charachter
*
* @param c character to return the lowercase representation of
*
* @return lowercase representation of c in ASCII
*/
int tolower(int c);

/**
* @brief Returns the uppercase representation of a charachter
*
* @param c character to return the uppercase representation of
*
* @return uppercase representation of c in ASCII
*/
int toupper(int c);

/**
* @brief Returns a string with the begining and ending whitespaces removed
*
* @param str the string to have white spaces removed from
*
* @return a sting with the begining and ending whitespaces removed
*/
char* trim(char * str);

#endif
