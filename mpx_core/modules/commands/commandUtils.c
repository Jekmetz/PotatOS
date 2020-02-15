#include <string.h>
#include <core/stdio.h>
#include <core/utility.h>
#include <core/io.h>
#include <stdarg.h>

#include "commands.h"
#include "commandUtils.h"
#include "time.h"
#include "../mpx_supt.h"

/**
* @brief A string to hold the command input up to the max command size
*/
char gparamstr[CMDSIZE];
/**
* @brief Will hold all the string pointers
*/
char* gparams[27];  // will hold all of the parameters

/**
* @file commands.c
*
* @brief Used as a helper function for set_flags
*
* @param alias alias to search for in aliases
* @param num_aliases number of aliases in aliases
* @param aliases array of ALIASes to search through
*
* @return charachter of flag that it found
*/
char set_flags_search_alias(char* alias, int num_aliases, ALIAS aliases[])
{
  unsigned char found = 0;
  char ret = '\0';
  for(int i = 0; i < num_aliases && !found; i++)
  {
    //if the alias or the character match...
    if(
      (strcmp(aliases[i].val, alias) == 0) ||
      (aliases[i].c == *alias && (*(alias + 1) == '\0') )
      )
    {
      found = 1;
      ret = aliases[i].c;
    }
  }

  return ret;
}

/**
* @file commands.c
*
* @brief Gets value of specific flag
*
* Usage: get_pvalue('a');
*
* @param c character of flag to get the value from
*
* @return value after the flag specified
*/
char* get_pvalue(char c)
{
  if(c == '\0') return gparams[26];
  else return gparams[alphanum(c)];
}

/**
* @file commands.c
*
* @brief sets flags based on param string, flags and num aliases
*
* Usage:
*    set_flags(paramstr,&flag,5,
*    'a',"alpha",
*    'b',"bravo",
*    'f',"foxtrot",
*    'g',"golf",
*    'r',"whiskey"
*    )
*
* @param paramstr string that each command gets. Typed by the user
* @param flag pointer to integer flag
* @param num_aliases number of aliases specified
*
* @return success or failure
*
* @note num_aliases must be the exact number of parameters. In the example, 5
*/
int set_flags(char* paramstr, int* flag, int num_aliases, ...)
{
  //Init Vars
  char* hold;
  int paramstrlen = strlen(paramstr);

  int loc = 0;
  char identChar;
  ALIAS aliases[num_aliases];

  //Init Var Vals
  *flag = 0;
  strcpy(gparamstr, paramstr);
  memset(gparams, '\0', 26 * sizeof(char*));

  //starting dynamic arguments
  va_list valist;
  va_start(valist, num_aliases);

  //load aliases!
  for(int i = 0; i < num_aliases; i++)
  {
    aliases[i].c = va_arg(valist, int);
    aliases[i].val = va_arg(valist,char*);
  }

  //end dynamic vars
  va_end(valist);

  //Handle NO_FLAG case specifically
  while( (loc < paramstrlen) && !isspace(gparamstr + loc) ) {loc++;}
  //Now we are at the first space...
  //until we hit our first non-space... advance loc
  while( (loc < paramstrlen) && isspace(gparamstr + loc)) {loc++;}
  //Now we are at the first character past the spaces
  if((loc < paramstrlen) && *(gparamstr + loc) != '-')
  {
    //set hold to the beginning
    hold = (gparamstr + loc);
    //We should handle the no flag case now
    while( (loc < paramstrlen) && (*(gparamstr + loc) != '-') ) {loc++;}
    //now we are at the next dash

    //if the end is not actually the end... make an end.
    if(*(gparamstr + loc) != '\0')
    {
      *(gparamstr + loc - 1) = '\0';
    }
    //now loc is at the first -'

    *flag |= NO_FLAG;
    gparams[26] = hold;
  }

  //until we hit a '-' or hit the end of the string...
  while( (loc < paramstrlen) && (*(gparamstr + loc) != '-') ) {loc++;}

  while( (loc < paramstrlen) )
  {
    //now we are at our first dash! Exciting!
    loc++; //advance to the next character

    //if this is a big alias name... advance to the beginning of the alias
    if(*(gparamstr + loc) == '-') { loc++; }
    hold = (gparamstr + loc);

    //until we hit our first space...
    while( (loc < paramstrlen) && !isspace(gparamstr + loc) ) {loc++;}

    //set that space to null to search for that alias starting at hold
    *(gparamstr + loc) = '\0';

    identChar = set_flags_search_alias(hold, num_aliases, aliases);

    //if an alias was not found... return failure
    if(identChar == '\0') return FAILURE;

    *flag |= 1 << alphanum(identChar);

    //if we are at the end of the string...
    if(loc == paramstrlen) break;

    //set it back to a space
    *(gparamstr + loc) = ' ';

    //until we hit our first non-space... advance loc
    while( (loc < paramstrlen) && isspace(gparamstr + loc)) {loc++;}

    //hold at the first non-space
    hold = (gparamstr + loc);

    //at this point, identChar is the correct character,
    //it is verified, and hold is at the first non-space

    //if we are at the first dash of another flag... continue
    if(*hold == '-') continue;

    //if we are not at a dash, there's data afoot or we're at the end!
    //if we're at the end... break;
    if(loc == paramstrlen) break;

    //if there's data afoot...
    while( (loc < paramstrlen) && *(gparamstr + loc) != '-')
    {
      //go ahuntin!
      loc++;
    }

    //if the end is not actually the end... make an end.
    if(*(gparamstr + loc) != '\0')
    {
      *(gparamstr + loc - 1) = '\0';
    }

    gparams[alphanum(identChar)] = hold;
  }

  return SUCCESS;
}