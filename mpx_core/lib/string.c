#include <stdarg.h>
#include <string.h>
#include <system.h>

#define F_MINUS (1 << 0)
#define F_PLUS (1 << 1)
#define F_PERCENT (1 << 2)

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

/*
  Procedure..: isdigit
  Description..: returns 1 if digit, else 0
  Params..: c-character to test
*/
int isdigit(char c) {
  return (c >= '0' && c <= '9');
}

/*
  Procedure..: reverse
  Description..: reverse a string from 0 to j
  Params..: str-string to reverse, j-index to reverse to
*/
char* reverse(char* str, int j) {
  int i = 0;
  while (i < j) {
    *(str + i) ^= *(str + j);
    *(str + j) ^= *(str + i);
    *(str + i++) ^= *(str + j--);
  }

  return str;
}

/*
  Procedure..: itoa
  Description..: converts integer to string
  Params..: num-number to convert to string, str-string to store it in,
  base-base of the number to convert
*/
char* itoa(int num, char* str, int base) {
  int idx = 0;
  int neg = 0;

  if (num == 0)  // if num is 0... handle specifically
  {
    str[idx++] = '0';
    str[idx] = '\0';
    return str;
  }

  if (num < 0 && base == 10)  // if num is negative and it is in base 10...
  {
    num = -num;
    neg = 1;
  }

  int r;
  while (num)  // while num is nonzero...
  {
    r = num % base;
    str[idx++] = (r > 9) ? (r - 10) + 'a' : r + '0';
    num = num / base;
  }
  if (neg)
    str[idx++] = '-';
  str[idx] = '\0';  // end it
  reverse(str, idx - 1);
  return str;
}

/*
  Procedure: buffer = sprintf_space_helper
  Description: adds spaces where needed for the sprintf function
  Params: buffer-char* to store spaces to, 
    fNum-format number, n-length of string that has been/will be
    added, doAction-boolean on whether or not to add the spaces
*/
char* sprintf_space_helper(char *buffer, int fNum, int n,
                          BYTE doAction) {
  if (doAction) // if we are to add the spaces...
  {
    int i;
    for (i = 0; i < fNum - n; i++) // for the amount of spaces to add to the buffer...
    {
      *buffer++ = ' '; // add spaces to the buffer
    }  
  }

  return buffer;
}

/*
  Procedure: sprintf
  Description: brings a lot of parameters into buffer based off of the format
    Format works for: %s, %d, %i, %c, and %x
  Params: buffer-char* to write to, format-format string, ...- parameters to match format
*/
int sprintf(char *buffer, char *format, ...) {
  int ret = 0;   // return value
  BYTE flag = 0; // keeps track of certain flags
  int fNum = 0;  // format number ex: %-10s
  char *temp;    // temp char* to use when needed

  va_list valist;
  va_start(valist, format); // start with the parameter nonsense

  while (*format) // while there is still stuff in the format...
  {
    if (*format != '%') // If the current token is a %...
    {
      *buffer++ = *format++; // add character to buffer
      continue;              // move on with your life
    }

    format++; // increment the format
    // if we need to evaluate a format...

    flag = 0;
    switch (*format) // switch through the first character
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
    while (isdigit(*format)) // while we have a digit...
    {
      fNum = fNum * 10 + (*format++ - '0');
    }

    if (flag & F_MINUS) // if we have a minus number...
    {
      fNum *= -1;
    }

    switch (*format) {

    // integer input
    case 'd':
    case 'i': {
      int num = va_arg(valist, int);
      int tmp = num;
      int n = 0;

      while (tmp) // while tmp is nonzero...
      {
        n++;            // add one to the length...
        tmp = tmp / 10; // divide by base 10
      }

      char str[n];
      itoa(num, str, 10); // store number in str

      // Add spaces in the front if applicable
      buffer = sprintf_space_helper(buffer, fNum, n, fNum > 0);

      int cnt = 0;
      while ((*buffer++ = str[cnt++]))
        ; // copy argument into buffer
      buffer--;

      // Add spaces in the back if applicable
      buffer = sprintf_space_helper(buffer, -fNum, n, fNum < 0);

      format++;
      break;
    }

    case 's': {
      temp = va_arg(valist, char *);
      int n = strlen(temp); // n is length of string

      // Add spaces in the front if applicable
      buffer = sprintf_space_helper(buffer, fNum, n, fNum > 0);

      while ((*buffer++ = *temp++))
        ; // copy argument into buffer
      buffer--;

      // Add spaces in the back if applicable
      buffer = sprintf_space_helper(buffer, -fNum, n, fNum < 0);

      format++;
      break;
    }

    case 'c': {
      // add character to buffer
      char c = va_arg(valist, int);

      // Add spaces in the front if applicable
      buffer = sprintf_space_helper(buffer, fNum, 1, fNum > 0);

      *buffer++ = c;

      // Add spaces in the back if applicable
      buffer = sprintf_space_helper(buffer, -fNum, 1, fNum < 0);
      format++;
      break;
    }

    case 'x': {
      int num = va_arg(valist, int);
      int tmp = num;
      int n = 0;

      while (tmp) // while tmp is nonzero...
      {
        n++;            // add one to the length
        tmp = tmp / 16; // divide by base 16
      }

      char str[n];
      itoa(num, str, 16); // put number into str

      // Add spaces in the front if applicable
      buffer = sprintf_space_helper(buffer, fNum, n, fNum > 0);

      int cnt = 0;
      while ((*buffer++ = str[cnt++]))
        ; // copy argument into buffer
      buffer--;

      // Add spaces in the back if applicable
      buffer = sprintf_space_helper(buffer, -fNum, n, fNum < 0);

      format++;
      break;
    }

    default: {
      if (flag & F_PERCENT) // if there were 2 percent signs...
      {
        *buffer++ = '%';
      } else // if there was a parameter type we did not realize...
      {
        format++;               // move on with your life
        va_arg(valist, void *); // skip that parameter
      }
    }
    }
  }

  *buffer = '\0'; // add null charachter at the end

  va_end(valist);
  return ret;
}
