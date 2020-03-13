#include <string.h>
#include <core/stdio.h>
#include <core/utility.h>
#include <core/io.h>
#include <stdarg.h>

#include "commands.h"
#include "commandUtils.h"
#include "time.h"
#include "../mpx_supt.h" 

/**
* @brief Array of COMMANDS that are supported
*/
COMMAND commands[] = {
  {"help", &cmd_help, NULL},
  {"version",&cmd_version, NULL},
  {"date",&cmd_date, NULL},
  {"time", &cmd_time, NULL},
  {"clear", &cmd_clear, NULL},
  //{"blockPCB", &cmd_blockPCB, NULL},
  {"resumePCB", &cmd_resume, NULL},
  {"suspendPCB", &cmd_suspend, NULL},
  {"showPCB", &cmd_show_pcb, NULL},
  {"showAllPCBs", &cmd_show_all_pcbs, NULL},
  {"showReadyPCBs", &cmd_show_ready_pcbs, NULL},
  {"showBlockedPCBs", &cmd_show_blocked_pcbs, NULL},
  //{"unblockPCB", &cmd_unblock_pcb, NULL},
  {"createPCB", &cmd_create_pcb, NULL},
  {"deletePCB", &cmd_delete_pcb, NULL},
  {"setPriorityPCB", &cmd_set_priority_pcb, NULL},
  {"loadr3", &cmd_loadr3, NULL},
  {"potat", &cmd_potat, NULL},
  //{"yield", &cmd_yield, NULL},
  {"alias", &cmd_alias, NULL},
  {"alarm", &cmd_alarm, NULL},
  {"infinity", &cmd_infinity, NULL},
  {"history", &cmd_history, NULL},
  {NULL, NULL, NULL} // leave NULL at the end for searching reasons
};

char gparamstr[CMDSIZE];

char* gparams[27];  // will hold all of the parameters

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

char* get_pvalue(char c)
{
  if(c == '\0') return gparams[26];
  else return gparams[alphanum(c)];
}

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

  //Handle NO_FLAG case specifically
  while( (loc < paramstrlen) && !isspace(gparamstr + loc) ) {loc++;}
  //Now we are at the first space...
  //until we hit our first non-space... advance loc
  while( (loc < paramstrlen) && isspace(gparamstr + loc)) {loc++;}
  //Now we are at the first character past the spaces
  if((loc < paramstrlen) && *(gparamstr + loc) != '-')
  {
    //set hold to the beginning
    hold = (gparamstr + loc);
    //We should handle the no flag case now
    while( (loc < paramstrlen) && (*(gparamstr + loc) != '-') ) {loc++;}
    //now we are at the next dash

    //if the end is not actually the end... make an end.
    if(*(gparamstr + loc) != '\0')
    {
      *(gparamstr + loc - 1) = '\0';
    }
    //now loc is at the first -'

    *flag |= NO_FLAG;
    gparams[26] = hold;
  }

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

/**
* @brief Finds which command in the global COMMANDS array
*
* @param cmd cmd typed by user
*/
COMMAND *search_commands(char* cmd) {
  int cmdidx;
  unsigned char found;
  char* cmdStart;
  char* testCmd;
  char* aliasCmd;
  // remove spaces from beginning of cmd
  while (*cmd == ' ') {
    cmd++;
  }

  cmdStart = cmd;  // save starting position for command
  cmdidx = 0;      // start at command 0
  found = 0;       // we didn't find anything yet

  // while we have another command ahead to process
  // and we have not found a match yet   
  while (commands[cmdidx].str != NULL && !found) {
    testCmd = commands[cmdidx].str;  //*testCmd is easier than writing *commands[cmdidx] :)

    /*******TEST ACTUAL COMMAND**********/

    // while each character matches and they are not null or space   
    while ((*testCmd == *cmd && !isnullorspace(*cmd))) {  // increment both
      testCmd++;
      cmd++;
    }

    if (isnullorspace(*testCmd) && isnullorspace(*cmd))  // if cmd is pointing to a space or a null  
    {  // this means that testCmd matched until a space and we have ourselves a
       // match!
      found = 1;
    } else  // if we did not find a match   
    {
      /**********TEST ALIAS COMMAND*************/
      cmd = cmdStart;
      aliasCmd = commands[cmdidx].alias;
      if(aliasCmd != NULL) //if an alias exists...
      {
        //while each character matches and they are not null or space
        while ((*aliasCmd == *cmd && !isnullorspace(*cmd))) { //increment both
          aliasCmd++;
          cmd++;
        }

        if(isnullorspace(*aliasCmd) && isnullorspace(*cmd))
        {
          //this means that aliasCmd matched until a space and we have ourselves a match!
          found = 1;
        }
      }
    }

    if(found == 0) cmdidx++;

    cmd = cmdStart;  // reset cmd back to the beginning
  }

  // if we did not find anything   
  if (commands[cmdidx].str == NULL)
    return NULL;  // set cmdidx to -1

  return &(commands[cmdidx]);
}

// Testing command, remove?
int showAll(){
  int cmdidx = 0;
  while (commands[cmdidx].str != NULL){
    printf("Native is %s\tAlias is %s\n",commands[cmdidx].str, commands[cmdidx].alias );
    cmdidx++;
  }

  return SUCCESS;
}