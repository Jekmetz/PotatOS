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
  chk = set_flags(trim(params),&flag,5,'l',"list",'s',"set",'r',"remove",'d',"date", 'c', "check");

  // If set flags fails
  if(chk != SUCCESS)
  {
    puts("\033[31mHouston, we have a problem. Check your flags!\033[0m");
    return FAILURE;
  }

  // If L flag used to list all alarms 
  if(flag & L_FLAG){
    listAlarms();
    return SUCCESS;
  }

  if((flag & S_FLAG) && (flag & D_FLAG)){
    char* tmp = get_pvalue('s');
    char* name = (char*)sys_alloc_mem(sizeof(ALARM) * (strlen(tmp) + 1));
    name = strcpy(name, tmp);

    cmd = get_pvalue('d');
    insertAlarm(name, cmd);
    return SUCCESS;
  }

  if(flag & S_FLAG){
    puts("You must also submit the date with the -d flag");
    return FAILURE;
  }

  if(flag & R_FLAG){
    cmd = get_pvalue('r');
    removeAlarm(cmd);
    //other();
    return SUCCESS;
  }

  if(flag & C_FLAG){
    check();
    return SUCCESS;
  }

  // if(flag & C_FLAG){
  //   check();


  //   return SUCCESS;
  // }

  ////if there was nothing specified...

  if(flag & NO_FLAG)
  {
    puts("Calling");
    return SUCCESS;
  }



  return SUCCESS;  // successful response
}