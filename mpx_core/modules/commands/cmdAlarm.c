#include <string.h>
#include <core/stdio.h>
#include <core/utility.h>
#include <core/io.h>
#include <stdarg.h>
#include <system.h>

#include "commands.h"
#include "commandUtils.h"
#include "time.h"
#include "../mpx_supt.h"
#include "../alarms/alarmWrangler.h"

int cmd_alarm(char* params) {
  // Init vars
  int flag = 0, chk;
  char *cmd;

  // trim and set flags
  chk = set_flags(trim(params),&flag,4,
    'l',"list",
    's',"set",
    'r',"remove",
    'd',"date"
    );

  // If set flags fails
  if(chk != SUCCESS)
  {
    puts("\033[31mHouston, we have a problem. Check your flags!\033[0m");
    return FAILURE;
  }

  if(strcmp(params,"alarm") == 0){
    puts("Refer to Alarm help page to learn command structure");
    return FAILURE;
  }

  // L Flag: list all alarms 
  if(flag & L_FLAG)
  {
    listAlarms();
    return SUCCESS;
  }

  // S & D Flag: Set a new alarm
  if((flag & S_FLAG) && (flag & D_FLAG)){
    // Getting the values for s which is the alarm message
    char* tmp = get_pvalue('s');

    if(tmp == NULL)
    {
      puts("\033[31mMessage was empty! Alarm not added!\033[0m");
      return FAILURE;
    }

    // Allocating memory
    char* str = (char*)sys_alloc_mem(sizeof(ALARM) * (strlen(tmp) + 1));
    str = strcpy(str, tmp);

    // Getting the values for d which is the date
    cmd = get_pvalue('d');

    // Inserting the alarm
    if(insertAlarm(str, cmd) == FAILURE)
    {
       puts("\033[31mAlarm not added!\033[0m");
       return FAILURE;
    }

    // Informing user
    puts("\033[32mAlarm successfully submitted!\033[0m");

    return SUCCESS;
  }

  // S Flag: Only s flag, must have d flag
  if(flag & S_FLAG){
    puts("\033[31mYou must also submit the date with the -d flag\033[0m");
    return FAILURE;
  }

  // R Flag: Remove alarm
  if(flag & R_FLAG){
    // Getting the values for r which is the alarm to be removed
    cmd = get_pvalue('r');

    // Calling remove alarm
    int ret = removeAlarm(cmd);

    if(ret == FAILURE){
      puts("\033[31mAlarm was not removed\033[0m");
      return FAILURE;
    }
    else{
      puts("\033[32mAlarm successfully removed\033[0m");
    }
    return SUCCESS;
  }

  // No Flag: Tell user to do it right
  if(flag & NO_FLAG)
  {
    puts("Refer to Alarm help page to learn command structure");
    return FAILURE;
  }

  return SUCCESS;  // successful response
}