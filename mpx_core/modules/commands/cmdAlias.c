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

int cmd_alias(char* params) {
  // Init vars
  int flag = 0, chk;
  char *cmd, *nativeCmd, *aliasCmd;

  // trim and set flags
  chk = set_flags(trim(params),&flag,0);

  // If flags fail
  if(chk != SUCCESS)
  {
    puts("\033[31mHouston, we have a problem. Check your flags!\033[0m");
    return FAILURE;
  }

  //if there was nothing specified...
  if(!(flag & NO_FLAG))
  {
    printf("No flag used");
    return SUCCESS;
  }

  // If there is a requested command, get it from the NO_VALUE using \0
  cmd = get_pvalue('\0');

  // Using strtok to get split the string on " "
  // we can call this because check flags enforces that at least one paramter is there
  char *token = strtok(cmd, " ");
  nativeCmd = token; // Assinging the split to the native command 

  // Testing, remove
  printf("First word is: %s\n",nativeCmd);
 
  // Attempting to split again, will not fail if this is not possible
  token = strtok(NULL, " ");

  // Checking to see if they split failed, this means that there is no new alias 
  if(strcmp(token, NULL) != 0){
    aliasCmd = token;

    // Testing, remove
    printf("Second word is: %s\n", aliasCmd);
  }
  // If it fails
  else{
    puts("\033[31mMust include the name for the new alias command!\033[0m");
    return FAILURE;
  }

  // Using seach_commands to find

  // Need to implement the code to put the new command into the commands array

  return SUCCESS;  // successful response
}