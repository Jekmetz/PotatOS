#include <string.h>
#include <core/stdio.h>
#include <core/utility.h>
#include <core/io.h>
#include <stdarg.h>

#include "commands.h"
#include "time.h"
#include "../mpx_supt.h"
#include "commandUtils.h"

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