#ifndef _STRING_H
#define _STRING_H

#include <system.h>

/*
  Procedure..: isspace
  Description..: Determine if a character is whitespace.
  Params..: c-character to check
*/
int isspace(const char *c);

/*
  Procedure..: memset
  Description..: Set a region of memory.
  Params..: s-destination, c-byte to write, n-count
*/
void* memset(void *s, int c, size_t n);

/*
  Procedure..: strcpy
  Description..: Copy one string to another.
  Params..: s1-destination, s2-source
*/
char* strcpy(char *s1, const char *s2);

/*
  Procedure..: strcat
  Description..: Concatenate the contents of one string onto another.
  Params..: s1-destination, s2-source
*/
char* strcat(char *s1, const char *s2);

/*
  Procedure..: strlen
  Description..: Returns the length of a string.
  Params..: s-input string
*/
int   strlen(const char *s);

/*
  Procedure..: strcmp
  Description..: String comparison
  Params..: s1-string 1, s2-string 2
*/
int   strcmp(const char *s1, const char *s2);

/*
  Procedure..: strtok
  Description..: Split string into tokens
  Params..: s1-string, s2-delimiter
*/
char* strtok(char *s1, const char *s2);

/*
  Procedure..: isdigit
  Description..: returns 1 if digit, else 0
  Params..: c-character to test
*/
int isdigit(char c);

/*
  Procedure..: reverse
  Description..: reverse a string from 0 to j
  Params..: str-string to reverse, j-index to reverse to
*/
char *reverse(char *str, int j);

/*
  Procedure..: atoi
  Description..: Convert an ASCII string to an integer
  Params..: const char *s -- String
*/
int atoi(const char *s);

/*
  Procedure..: itoa
  Description..: converts integer to string
  Params..: num-number to convert to string, str-string to store it in,
  base-base of the number to convert
*/
char *itoa(int num, char *str, int base);

/* And finally....
   For the brave ones! (Note: you'll need to add a prototype to string.h)
   sprintf must work properly for the following types to receive extra credit:
     1) characters
     2) strings
     3) signed integers
     4) hexadecimal numbers may be useful
     ...
     \infty) Or feel free to completely implement sprintf
             (Read the man Page: $ man sprintf)
   int sprintf(char *str, const char *format, ...);
*/

int sprintf(char *buffer, char *format, ...);

#endif
