#include <string.h>
#include <core/stdio.h>
#include <core/utility.h>
#include <core/io.h>
#include <stdarg.h>

#include "help.h"
#include "commands.h"
#include "commandUtils.h"
#include "time.h"
#include "../mpx_supt.h"

#define CMDSIZE 100

/**
* @brief A string to hold the command input up to the max command size
*/
extern char gparamstr[CMDSIZE];
/**
* @brief Will hold all the string pointers
*/
extern char* gparams[26];  // will hold all of the parameters

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
    return SUCCESS;
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

  else {
    puts("\033[31mNo help available for that command.\033[0m");
  }

    return SUCCESS;  // successful response
  }