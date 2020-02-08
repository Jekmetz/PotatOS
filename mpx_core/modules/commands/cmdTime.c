/**
* @file cmdTime.c
*
* @brief Time Command
*/

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