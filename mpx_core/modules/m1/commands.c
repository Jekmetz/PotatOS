//int validate_date(int months, int day, int year);

/*************************************************************
*	This C file contains all of the commands that will be used
*  by the command handler. All of the commands in this file
*  should have the syntax:
*  int cmd_<cmd_name>(char* params);
**************************************************************/
#include <string.h>
#include <core/serial.h>
#include <core/utility.h>
#include <core/io.h>
#include <stdarg.h>

#include "help.h"
#include "commands.h"
#include "time.h"
#include "../mpx_supt.h"

#define CMDSIZE 100

#define SUCCESS 0
#define FAILURE -1
#define MAXPARAMCOUNT 10

// cmd_help flags
#define A_FLAG (1 << 0)
#define B_FLAG (1 << 1)
#define C_FLAG (1 << 2)
#define D_FLAG (1 << 3)
#define E_FLAG (1 << 4)
#define F_FLAG (1 << 5)
#define G_FLAG (1 << 6)
#define H_FLAG (1 << 7)
#define I_FLAG (1 << 8)
#define J_FLAG (1 << 9)
#define K_FLAG (1 << 10)
#define L_FLAG (1 << 11)
#define M_FLAG (1 << 12)
#define N_FLAG (1 << 13)
#define O_FLAG (1 << 14)
#define P_FLAG (1 << 15)
#define Q_FLAG (1 << 16)
#define R_FLAG (1 << 17)
#define S_FLAG (1 << 18)
#define T_FLAG (1 << 19)
#define U_FLAG (1 << 20)
#define V_FLAG (1 << 21)
#define W_FLAG (1 << 22)
#define Y_FLAG (1 << 23)
#define X_FLAG (1 << 24)
#define Z_FLAG (1 << 25)

#define alphanum(c) (('a' <= c && c <= 'z') ? c - 'a' : c - 'A')

typedef struct {
  char c;
  char* val;
} ALIAS;

//Prototypes for private functions
/*Procedure: set_flags
  Description: sets flags based on param string, flags and num aliases
  Usage: 
    set_flags(paramstr,&flag,5,
    'a',"alpha",
    'b',"bravo",
    'f',"foxtrot",
    'g',"golf",
    'r',"whiskey"
    )
 */
int set_flags(char* paramstr, int* flag, int num_aliases, ...);

/*
  Procedure: get_pvalue
  Description: used to get value for certain flag
  Usage: get_pvalue('a') for a's value
*/
char* get_pvalue(char c);

/*
  Procedure: set_flags_search_alias
  Description: helps set-flags by searching alias list
               to see if there exists an alias in it
*/
char set_flags_search_alias(char* alias, int num_aliases, ALIAS aliases[]);


// Global variables
char gparamstr[CMDSIZE];
char* gparams[26];  // will hold all of the parameters

// private function prototypes

/*
Procedure: cmd_help
Params: params-will serve as the params for each of these things
*/
int cmd_help(char* params) {
  // Init vars
  int flag = 0, chk;

  chk = set_flags(params, &flag, 2,
    'f',"full",
    'c',"cmd"
    );

  if (chk != SUCCESS) {
    char *ret = "\033[31mHouston, we have a problem. Check your flags!\033[0m";
    int size = strlen(ret);
    sys_req(WRITE,DEFAULT_DEVICE,ret,&size);
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

// Print the current version of the mpx
int cmd_version(char* params) {
  // Init vars
  int flag = 0, chk;

  chk = set_flags(params, &flag, 1,
    'f',"full"
    );

  if (chk != SUCCESS) {
    char *ret = "\033[31mHouston, we have a problem. Check your flags!\033[0m";
    int size = strlen(ret);
    sys_req(WRITE,DEFAULT_DEVICE,ret,&size);
    return FAILURE;
  }

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


int cmd_date(char* params) {
  // Init vars
  int flag = 0, chk;

  chk = set_flags(params, &flag, 1,
    's',"set"
    );

  if (chk != SUCCESS) {
    char *ret = "\033[31mHouston, we have a problem. Check your flags!\033[0m";
    int size = strlen(ret);
    sys_req(WRITE,DEFAULT_DEVICE,ret,&size);
    return FAILURE;
  }

  //get flag values
  char* date = get_pvalue('s');

  if(flag&S_FLAG) //if they are setting the date...
  {
    if(date == NULL)
    {
      char *ret = "\033[31mMust include time value after [-s | --set]! Exiting!\033[0m";
      int size = strlen(ret);
      sys_req(WRITE,DEFAULT_DEVICE,ret,&size);
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
  char buff[64];
  sprintf(buff, "%d/%d/%d", tim.day_of_month, tim.month, tim.year);
  serial_println(buff);

  return SUCCESS;
}

int cmd_time(char* params) {
  // Init vars
  int flag = 0, chk;

  chk = set_flags(params, &flag, 2,
    's',"set"
    );

  if (chk != SUCCESS) {
    char *ret = "\033[31mHouston, we have a problem. Check your flags!\033[0m";
    int size = strlen(ret);
    sys_req(WRITE,DEFAULT_DEVICE,ret,&size);
    return FAILURE;
  }

  //get flag values
  char *time = get_pvalue('s');

  if(flag & S_FLAG)
  {
      if(time == NULL)
      {
        char *ret = "\033[31mMust include time value after [-s | --set]! Exiting!\033[0m";
        int size = strlen(ret);
        sys_req(WRITE,DEFAULT_DEVICE,ret,&size);
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
  char form_tim[64];
  sprintf(form_tim, "%d:%d:%d", tim.hours, tim.minutes, tim.seconds);
  serial_println(form_tim);

  return SUCCESS;
}

int cmd_clear(char* params)
{
  //Unused... supresses warning
  if(params){}

  int size = 10;
  sys_req(WRITE,DEFAULT_DEVICE,"\033[2J\033[H",&size);

  return SUCCESS;
}

/*
  Procedure: set_flags_search_alias
  Description: helps set-flags by searching alias list
               to see if there exists an alias in it
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

/*
  Procedure: get_pvalue
  Description: used to get value for certain flag
  Usage: get_pvalue('a') for a's value
*/
char* get_pvalue(char c)
{
  return gparams[alphanum(c)];
}

/*Procedure: set_flags
  Description: sets flags based on param string, flags and num aliases
  Usage: 
    set_flags(paramstr,&flag,5,
    'a',"alpha",
    'b',"bravo",
    'f',"foxtrot",
    'g',"golf",
    'r',"whiskey"
    )
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