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
  chk = set_flags(trim(params),&flag,
    1,
    'l',"list"
    );

  // If flags fail
  if(chk != SUCCESS)
  {
    puts("\033[31mHouston, we have a problem. Check your flags!\033[0m");
    return FAILURE;
  }

  if(flag & L_FLAG) //list
  {
    //loop through all commands and list the aliases
    puts("");
    printf(
      "______________________________________\n"
      "|%-20s|%-15s|\n"
      "|====================|===============|\n"
      ,"command","alias");

    COMMAND* trav = get_command_array();

    while(trav->str != NULL)
    {
      if(trav->alias != NULL)
      {
        printf(
          "|%-20s|%-15s|\n"
          "|--------------------|---------------|\n"
          ,trav->str, trav->alias);
      }
      trav = trav + 1;
    }

    printf("|====================================|");
    return SUCCESS;
  }

  //if there was nothing specified...
  if(!(flag & NO_FLAG))
  {
    puts("You must include the <native name> and the <alias name>");
    return SUCCESS;
  }

  // If there is a requested command, get it from the NO_VALUE using \0
  cmd = get_pvalue('\0');

  // Using strtok to get split the string on " "
  // we can call this because check flags enforces that at least one paramter is there
  char *token = strtok(cmd, " ");
  nativeCmd = token; // Assinging the split to the native command 
 
  // Attempting to split again, will not fail if this is not possible
  token = strtok(NULL, " ");

  // Checking to see if they split failed, this means that there is no new alias 
  if(strcmp(token, NULL) != 0){
    aliasCmd = token;
  }
  // If it fails
  else{
    puts("\033[31mMust include the name for the new alias command!\033[0m");
    return FAILURE;
  }

  // Using seach_commands to find
  COMMAND *ret = search_commands(nativeCmd);

  if(ret == FAILURE){
    printf("'%s' is not a valid command\n", nativeCmd);
    return FAILURE;
  }
  else {
    // If we're overwriting an alias, make sure to free the previous alias
    if (ret->alias != NULL) {
      sys_free_mem(ret->alias);
    }

    char* name = (char*)sys_alloc_mem(sizeof(char) * (strlen(aliasCmd) + 1));
    name = strcpy(name, aliasCmd);

    ret->alias = name;
    printf("'%s' is aliased as '%s'\n", nativeCmd, aliasCmd);
    return SUCCESS;
  }
}