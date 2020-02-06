/**
* @file commands.c
*
* @brief This file contains all the commands that will be used by the command handler
*/

/*************************************************************
*	This C file contains all of the commands that will be used
*  by the command handler. All of the commands in this file
*  should have the syntax:
*  int cmd_<cmd_name>(char* params);
**************************************************************/
#include <string.h>
#include <core/stdio.h>
#include <core/utility.h>
#include <core/io.h>
#include <stdarg.h>

#include "help.h"
#include "commands.h"
#include "time.h"
#include "../mpx_supt.h"

/**
* @brief The command input buffer
*
* This a macro to store the command input buffer. Here we can change
* the ammount of characters we allow to be entered into the command
* handler at once. We currently allow 100 characters.
*/
#define CMDSIZE 100

/**
* @brief Macro to return a 0 on success
*/
#define SUCCESS 0
/**
* @brief Macro to return a -1 on failure
*/
#define FAILURE -1
/**
* @brief The maximum parameters allowed per command
*/
#define MAXPARAMCOUNT 10

/** cmd_help flags */
/** A flag binary bit shift macro */
#define A_FLAG (1 << 0)
/** B flag binary bit shift macro */
#define B_FLAG (1 << 1)
/** C flag binary bit shift macro */
#define C_FLAG (1 << 2)
/** D flag binary bit shift macro */
#define D_FLAG (1 << 3)
/** E flag binary bit shift macro */
#define E_FLAG (1 << 4)
/** F flag binary bit shift macro */
#define F_FLAG (1 << 5)
/** G flag binary bit shift macro */
#define G_FLAG (1 << 6)
/** H flag binary bit shift macro */
#define H_FLAG (1 << 7)
/** I flag binary bit shift macro */
#define I_FLAG (1 << 8)
/** J flag binary bit shift macro */
#define J_FLAG (1 << 9)
/** K flag binary bit shift macro */
#define K_FLAG (1 << 10)
/** L flag binary bit shift macro */
#define L_FLAG (1 << 11)
/** M flag binary bit shift macro */
#define M_FLAG (1 << 12)
/** N flag binary bit shift macro */
#define N_FLAG (1 << 13)
/** O flag binary bit shift macro */
#define O_FLAG (1 << 14)
/** P flag binary bit shift macro */
#define P_FLAG (1 << 15)
/** Q flag binary bit shift macro */
#define Q_FLAG (1 << 16)
/** R flag binary bit shift macro */
#define R_FLAG (1 << 17)
/** S flag binary bit shift macro */
#define S_FLAG (1 << 18)
/** T flag binary bit shift macro */
#define T_FLAG (1 << 19)
/** U flag binary bit shift macro */
#define U_FLAG (1 << 20)
/** V flag binary bit shift macro */
#define V_FLAG (1 << 21)
/** W flag binary bit shift macro */
#define W_FLAG (1 << 22)
/** Y flag binary bit shift macro */
#define Y_FLAG (1 << 23)
/** X flag binary bit shift macro */
#define X_FLAG (1 << 24)
/** Z flag binary bit shift macro */
#define Z_FLAG (1 << 25)
/** A flag binary bit shift macro */

/**
* @brief A helper macro that will take a letter and return its integer equivelent
*
* This is a helper macro that is used in set_flags and get_gparams. It takes in
* character and return the integer equivalent of that character.
*
* @param c The character to be returned as an int
*/
#define alphanum(c) (('a' <= c && c <= 'z') ? c - 'a' : c - 'A')

/**
* @brief A struct to hold command aliases
*
* The ALIAS Struct is a custom struct that is designed to hold aliases for commands
*
* @param c A string that will hold the initial command name
* @param val A string pointer that will point to the original command name
*/
typedef struct {
  char c;
  char* val;
} ALIAS;


/**
* @brief Sets flags based on param string, flags and num aliases
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
int set_flags(char* paramstr, int* flag, int num_aliases, ...);

/**
* @brief Gets value of specific flag
*
* Usage: get_pvalue('a');
*
* @param c character of flag to get the value from
*
* @return value after the flag specified
*/
char* get_pvalue(char c);

/**
* @brief Used as a helper function for set_flags
*
* @param alias alias to search for in aliases
* @param num_aliases number of aliases in aliases
* @param aliases array of ALIASes to search through
*
* @return charachter of flag that it found
*/
char set_flags_search_alias(char* alias, int num_aliases, ALIAS aliases[]);


/**
* @brief A string to hold the command input up to the max command size
*/
char gparamstr[CMDSIZE];
/**
* @brief Will hold all the string pointers
*/
char* gparams[26];  // will hold all of the parameters


/**
* @brief The help command will show a page to assist users with commands
*
* The help command can be called to do one of two things
*   List all the commands that have help pages
*   Request a help page for a certain command
*
*
* @param params param string typed by user
*
* @return A help page
*/
int cmd_help(char* params) {
  // Init vars
  int flag = 0, chk;

  chk = set_flags(params, &flag, 2,
    'f',"full",
    'c',"cmd"
    );

  if (chk != SUCCESS) {
    puts("\033[31mHouston, we have a problem. Check your flags!\033[0m");
    return FAILURE;
  }

  //get flag values
  char* cmd = get_pvalue('c');

  // lists all the commands that have help pages, help without pararms
  if( !(flag&C_FLAG) ){
    // Help without arguments
    HELP;
  }

  // Help page for help
  if (strcmp(cmd, "help") == 0) {
    // If no full flag used
    if( !(flag&F_FLAG) ){
     HELP_HELP;
    }
    //If full flag was used
    else
    {
     HELP_HELP_FULL;
    }
  }

  // Help page for version
  else if (strcmp(cmd, "version") == 0) {
    // If no flag set
    if( !(flag&F_FLAG) ){
      HELP_VERSION;
    }
    // If full flag was used
    else
    {
      HELP_VERSION_FULL;
    }
  }
  // Help page for shutdown
  else if (strcmp(cmd, "shutdown") == 0) {
    // If no flag is set
    if( !(flag&F_FLAG) ){
      HELP_SHUTDOWN;
    }
    // If full flag is set
    else
    {
      HELP_SHUTDOWN_FULL;
    }
  }

  // Help page for date
  else if (strcmp(cmd, "date") == 0) {
    // If no flag is set
    if( !(flag&F_FLAG) ){
      HELP_DATE;
    }
    // If full flag was used
    else
    {
      HELP_DATE_FULL;
    }
  }

  // Help page for time
  else if (strcmp(cmd, "time") == 0) {
    // If not flag is set
    if( !(flag&F_FLAG) ){
      HELP_TIME;
    }
    // If full flag was used
    else
    {
      HELP_TIME_FULL;
    }
  }

    return SUCCESS;  // successful response
  }

/**
* @brief The version command will show the version information
*
* The version command can be called to display the version information.
* The shortned return will just show the short version.
* The long return will include the current module, the version, and the
* contributing developers
*
* @param params param string typed by user
*
* @return A version page
*/
int cmd_version(char* params) {
  // Init vars
  int flag = 0, chk;

  chk = set_flags(params, &flag, 1,
    'f',"full"
  );

  if (chk != SUCCESS) {
    puts("\033[31mHouston, we have a problem. Check your flags!\033[0m");
    return FAILURE;
  }

  // char* cmd = get_pvalue('f');

  //get flag values
  // If no full flag set
  if( !(flag & F_FLAG)){
	   VERSION;
  }
  // If full flag used
  else{
    VERSION_FULL;
  }

  return SUCCESS;
}

/**
* @brief The date command will do one of two things.
*   Show the current system date
*   Set a new system date
*
* The date command can be used to query the systems RTC to display the current
* date. It can also be used to set the systems RTC to a desired date. There is
* code to check for illegal dates such as Feb 30 on a non leap year.
*
* @param params param string typed by user
*
* @return The current system date
*
* @warning The RTC only allows dates between 1700-2999
*/
int cmd_date(char* params) {
  // Init vars
  int flag = 0, chk;

  chk = set_flags(params, &flag, 1,
    's',"set"
    );

  if (chk != SUCCESS) {
    puts("\033[31mHouston, we have a problem. Check your flags!\033[0m");
    return FAILURE;
  }

  //get flag values
  char* date = get_pvalue('s');

  if(flag&S_FLAG) //if they are setting the date...
  {
    if(date == NULL)
    {
      puts("\033[31mMust include time value after [-s | --set]! Exiting!\033[0m");
      return FAILURE;
    }

    char *aday = strtok(date, "/");
    char *amonth = strtok(NULL, "/");
    char *ayear = strtok(NULL, "/");

    time_h n_date = {-1,-1,-1,-1,-1,-1};
    n_date.day_of_month = atoi(aday);
    n_date.month = atoi(amonth);
    n_date.year = atoi(ayear);
    set_current_time(n_date);
  }

  // printing the date
  time_h tim = get_current_time();
  printf("%d/%d/%d\n", tim.day_of_month, tim.month, tim.year);
  return SUCCESS;
}


/**
* @brief The time command will do one of two things.
*   Show the current system time
*   Set a new system time
*
* The time command can be used to query the systems RTC to display the current
* time. It can also be used to set the systems RTC to a desired time. There is
* code to check for illegal times.
*
* @param params param string typed by user
*
* @return The current system time
*
* @note The time is kept in 24 hour time
*/
int cmd_time(char* params) {
  // Init vars
  int flag = 0, chk;

  chk = set_flags(params, &flag, 2,
    's',"set"
    );

  if (chk != SUCCESS) {
    puts("\033[31mHouston, we have a problem. Check your flags!\033[0m");
    return FAILURE;
  }

  //get flag values
  char *time = get_pvalue('s');

  if(flag & S_FLAG)
  {
      if(time == NULL)
      {
        puts("\033[31mMust include time value after [-s | --set]! Exiting!\033[0m");
        return FAILURE;
      }

      char *ahour = strtok(time, ":");
      char *aminute = strtok(NULL, ":");
      char *asecond = strtok(NULL, ":");

      time_h n_date = {-1,-1,-1,-1,-1,-1};
      n_date.seconds = atoi(asecond);
      n_date.minutes = atoi(aminute);
      n_date.hours = atoi(ahour);
      set_current_time(n_date);
  }

  time_h tim = get_current_time();
  printf("%02d:%02d:%02d\n", tim.hours, tim.minutes, tim.seconds);

  return SUCCESS;
}

/**
* @file commands.c
*
* @brief clears the screen and sets the pointer at home
*
* @param params param string typed by user
*
* @return SUCCESS or FAILURE
*/
int cmd_clear(char* params)
{
  //Unused... supresses warning
  if(params){}

  puts("\033[2J\033[H");

  return SUCCESS;
}

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
  return gparams[alphanum(c)];
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
