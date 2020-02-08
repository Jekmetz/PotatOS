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
#include "commandUtils.h"

/**
* @brief The version command will show the version information
*
* The version command can be called to display the version information.
* The shortened return will just show the short version.
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

