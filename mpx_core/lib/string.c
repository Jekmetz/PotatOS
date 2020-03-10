/**
* @file string.c
*
* @brief Holds all utility functions used to modify strings
*/ 

#include <string.h>
#include <stdarg.h>
#include <system.h>

#define F_MINUS (1 << 0)
#define F_PLUS (1 << 1)
#define F_PERCENT (1 << 2)
#define F_ZERO (1 << 3)


typedef unsigned char BYTE;

/* ************************************ *
 * Author:  Forrest Desjardins    	*
 * Creation Date:  2014           	*
 * 				  	*
 *   DISCLAIMER!!!!!:  		  	*
 *  The following methods are provided	*
 *  for your use as part of the MPX	*
 *  project. While we have tested these	*
 *  We make certification of 	 	*
 *  complete correctness.		*
 *   Additionally not all methods are 	*
 *  implemented :  See:  itoa		*
 * ************************************ */

/*
  Procedure..: strlen
  Description..: Returns the length of a string.
  Params..: s-input string
*/
int strlen(const char* s) {
  int r1 = 0;
  if (*s)
    while (*s++)
      r1++;
  return r1;  // return length of string
}

/*
  Procedure..: strcpy
  Description..: Copy one string to another.
  Params..: s1-destination, s2-source
*/
char* strcpy(char* s1, const char* s2) {
  char* rc = s1;
  while ((*s1++ = *s2++))
    ;
  return rc;  // return pointer to destination string
}

/*
  Procedure..: atoi
  Description..: Convert an ASCII string to an integer
  Params..: const char *s -- String
*/ 
int atoi(const char* s) {
  int res = 0;
  int charVal = 0;
  char sign = ' ';
  char c = *s;

  while (isspace(&c)) {
    ++s;
    c = *s;
  }  // advance past whitespace

  if (*s == '-' || *s == '+')
    sign = *(s++);  // save the sign

  while (*s != '\0') {
    charVal = *s - 48;
    res = res * 10 + charVal;
    s++;
  }

  if (sign == '-')
    res = res * -1;

  return res;  // return integer
}
/*
  Procedure..: strcmp
  Description..: String comparison
  Params..: s1-string 1, s2-string 2
*/
int strcmp(const char* s1, const char* s2) {
  // Remarks:
  // 1) If we made it to the end of both strings (i. e. our pointer points to a
  //    '\0' character), the function will return 0
  // 2) If we didn't make it to the end of both strings, the function will
  //    return the difference of the characters at the first index of
  //    indifference.
  while ((*s1) && (*s1 == *s2)) {
    ++s1;
    ++s2;
  }
  return (*(unsigned char*)s1 - *(unsigned char*)s2);
}

/* ---------------------------------------
    Functions below this point are given.
    No need to tamper with these!
   --------------------------------------- */

/*
  Procedure..: strcat
  Description..: Concatenate the contents of one string onto another.
  Params..: s1-destination, s2-source
*/
char* strcat(char* s1, const char* s2) {
  char* rc = s1;
  if (*s1)
    while (*++s1)
      ;
  while ((*s1++ = *s2++))
    ;
  return rc;
}

/*
  Procedure..: isspace
  Description..: Determine if a character is whitespace.
  Params..: c-character to check
*/
int isspace(const char* c) {
  if (*c == ' ' || *c == '\n' || *c == '\r' || *c == '\f' || *c == '\t' ||
      *c == '\v') {
    return 1;
  }
  return 0;
}

/*
  Procedure..: memset
  Description..: Set a region of memory.
  Params..: s-destination, c-byte to write, n-count
*/
void* memset(void* s, int c, size_t n) {
  unsigned char* p = (unsigned char*)s;
  while (n--) {
    *p++ = (unsigned char)c;
  }
  return s;
}

/*
  Procedure..: strtok
  Description..: Split string into tokens
  Params..: s1-string, s2-delimiter
*/
char* strtok(char* s1, const char* s2) {
  static char* tok_tmp = NULL;
  const char* p = s2;

  // new string
  if (s1 != NULL) {
    tok_tmp = s1;
  }
  // old string cont'd
  else {
    if (tok_tmp == NULL) {
      return NULL;
    }
    s1 = tok_tmp;
  }

  // skip leading s2 characters
  while (*p && *s1) {
    if (*s1 == *p) {
      ++s1;
      p = s2;
      continue;
    }
    ++p;
  }

  // no more to parse
  if (!*s1) {
    return (tok_tmp = NULL);
  }

  // skip non-s2 characters
  tok_tmp = s1;
  while (*tok_tmp) {
    p = s2;
    while (*p) {
      if (*tok_tmp == *p++) {
        *tok_tmp++ = '\0';
        return s1;
      }
    }
    ++tok_tmp;
  }

  // end of string
  tok_tmp = NULL;
  return s1;
}

////////////////////self added

/**
* @brief Checks if char c is a digit
*
* @param c character to check
*
* @return is digit: 1; is not digit: 0;
*/
int isdigit(char c) {
  return (c >= '0' && c <= '9');
}

/**
* @brief reverse a string from 0 to j
*
* @param str string to reverse
* @param j index to reverse str to
*
* @return pointer to str
*/
char* reverse(char* str, int end) {
  for (int front = 0; front < end; front++) {
    str[front] ^= str[end];
    str[end] ^= str[front];
    str[front] ^= str[end];

    front += 1;
    end -= 1;
  }

  return str;
}

/**
* @brief Converts signed integer to string
*
* @param num number to convert
* @param str string to store result in
* @param base base to convert to
*
* @return pointer to str
*/
char* itoa(int num, char* str, int base) {
  if (num < 0 && base == 10) {
    str[0] = '-';
    utoa(-num, str + 1, base);
  } else {
    utoa(num, str, base);
  }

  return str;
}

/**
* @brief Converts unsigned integer to string
*
* @param num number to convert
* @param str string to store result in
* @param base base to convert to
*
* @return pointer to str
*/
char* utoa(u32int num, char* str, int base) {
  int idx = 0;

  int digit;

  do {
    digit = num % base;
    num = num / base;

    if (digit > 9) {
      str[idx] = (digit - 10) + 'a';
    } else {
      str[idx] = digit + '0';
    }

    idx += 1;
  } while (num != 0);

  str[idx] = '\0';
  reverse(str, idx - 1);
  return str;
}

/**
* @brief adds spaces where needed for the sprintf function
*
* @param buffer character pointer to store spaces to
* @param pad what character to pad with
* @param fNum format number from sprintf
* @param n length of string that has been/will be added
* @param doAction boolean on whether or not to add the spaces
*
* @return pointer to buffer
*/
char* sprintf_pad_helper(char* buffer, char pad, int fNum, int n, BYTE doAction) {
  if (doAction)  // if we are to add the spaces...
  {
    int i;
    for (i = 0; i < fNum - n;
         i++)  // for the amount of spaces to add to the buffer...
    {
      *buffer++ = pad;  // add pad to the buffer
    }
  }

  return buffer;
}

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
int sprintf_internal(char *buffer, char *format, va_list valist)
{
  int ret = 0;    // return value
  BYTE flag = 0;  // keeps track of certain flags
  int fNum = 0;   // format number ex: %-10s
  char* temp;     // temp char* to use when needed

    while (*format)  // while there is still stuff in the format...
  {
    if (*format != '%')  // If the current token is a %...
    {
      *buffer++ = *format++;  // add character to buffer
      continue;               // move on with your life
    }

    format++;  // increment the format
    // if we need to evaluate a format...

    flag = 0;

    //Handle 0
    if(*format == '0')
    {
      flag |= F_ZERO;
      format++;
    }

    switch (*format)  // switch through the first character
    {
      case '+':
        flag |= F_PLUS;
        format++;
        break;
      case '-':
        flag |= F_MINUS;
        format++;
        break;
      case '%':
        flag |= F_PERCENT;
        format++;
        break;
    }

    fNum = 0;

    if(!(flag&F_PERCENT)) //if we had something other than a percent...
    {
      //Set zero pad or not if it is a plus or a minus
      if(flag&F_PLUS || flag & F_MINUS)
      {
        if(*format == '0')  //if it is a zero...
        {
          //act accordingly
          flag |= F_ZERO;
          format++;
        }
      }

      while (isdigit(*format))  // while we have a digit...
      {
        fNum = fNum * 10 + (*format++ - '0');
      }

      if (flag & F_MINUS)  // if we have a minus number...
      {
        fNum *= -1;
      }
    }

    switch (*format) {
      // integer input
      case 'd':
      case 'i': {
        int num = va_arg(valist, int);
        int tmp = num;
        int n = 0;

        do {
          n += 1;           // add one to the length...
          tmp = tmp / 10;   // divide by base 10
        } while (tmp != 0); // while tmp is nonzero...

        char str[n];
        itoa(num, str, 10);  // store number in str

        // Add spaces in the front if applicable
        buffer = sprintf_pad_helper(buffer, (flag&F_ZERO ? '0':' '), fNum, n, fNum > 0);

        int cnt = 0;
        while ((*buffer++ = str[cnt++]))
          ;  // copy argument into buffer
        buffer--;

        // Add spaces in the back if applicable
        buffer = sprintf_pad_helper(buffer, (flag&F_ZERO ? '0':' '), -fNum, n, fNum < 0);

        format++;
        break;
      }

      case 's': {
        temp = va_arg(valist, char*);
        int n = strlen(temp);  // n is length of string

        // Add spaces in the front if applicable
        buffer = sprintf_pad_helper(buffer, (flag&F_ZERO ? '0':' '), fNum, n, fNum > 0);

        while ((*buffer++ = *temp++));  // copy argument into buffer
        buffer--;

        // Add spaces in the back if applicable
        buffer = sprintf_pad_helper(buffer, (flag&F_ZERO ? '0':' '), -fNum, n, fNum < 0);

        format++;
        break;
      }

      case 'c': {
        // add character to buffer
        char c = va_arg(valist, int);

        // Add spaces in the front if applicable
        buffer = sprintf_pad_helper(buffer, (flag&F_ZERO ? '0':' '), fNum, 1, fNum > 0);

        *buffer++ = c;

        // Add spaces in the back if applicable
        buffer = sprintf_pad_helper(buffer, (flag&F_ZERO ? '0':' '), -fNum, 1, fNum < 0);
        format++;
        break;
      }

      case 'x': {
        int num = va_arg(valist, int);
        int tmp = num;
        int n = 0;

        do {
          n += 1;           // add one to the length...
          tmp = tmp / 10;   // divide by base 10
        } while (tmp != 0); // while tmp is nonzero...

        char str[n];
        itoa(num, str, 16);  // put number into str

        // Add spaces in the front if applicable
        buffer = sprintf_pad_helper(buffer, (flag&F_ZERO ? '0':' '), fNum, n, fNum > 0);

        int cnt = 0;
        while ((*buffer++ = str[cnt++]))
          ;  // copy argument into buffer
        buffer--;

        // Add spaces in the back if applicable
        buffer = sprintf_pad_helper(buffer, (flag&F_ZERO ? '0':' '), -fNum, n, fNum < 0);

        format++;
        break;
      }

      default: {
        if (flag & F_PERCENT)  // if there were 2 percent signs...
        {
          *buffer++ = '%';
        } else  // if there was a parameter type we did not realize...
        {
          format++;               // move on with your life
          va_arg(valist, void*);  // skip that parameter
        }
      }
    }
  }

  *buffer = '\0';  // add null charachter at the end
  return ret;
}

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
int sprintf(char* buffer, char* format, ...) {
  va_list valist;
  va_start(valist, format);  // start with the parameter nonsense
  int ret = sprintf_internal(buffer, format, valist);
  va_end(valist);
  return ret;
}

/**
* @brief Returns the lowercase representation of a charachter
*
* @param c character to return the lowercase representation of
*
* @return lowercase representation of c in ASCII
*/
char tolower(char c)
{
  return ( ('A' <= c && c <= 'Z') ? c + ('a' - 'A') : c);
}

/**
* @brief Returns the uppercase representation of a charachter
*
* @param c character to return the uppercase representation of
*
* @return uppercase representation of c in ASCII
*/
char toupper(char c)
{
  return ( ('a' <= c && c <= 'z') ? c - ('a' - 'A') : c);
}

/**
* @brief Returns a string with the begining and ending whitespaces removed
*
* @param str the string to have white spaces removed from
*
* @return a sting with the begining and ending whitespaces removed
*/
char* trim(char * str){
  // Character pointer to the end of the string str  
  char* end;

  // Zero length string case
  if(strlen(str) == 0){
    return str;
  }

  // Trimming whitespace at beginning
  while(isspace(str)){
    str++;
  }

  // Trimming whitespace at end
  end = str + strlen(str) - 1;
  while(end > str && isspace(end)){
    end--;
  }

  // Capping string with null char
  end[1] = '\0';

  // Returning the altered string str
  return str;
}
