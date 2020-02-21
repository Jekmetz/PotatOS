/**
* @file command_handler c
*
* @brief The primary command handler for the Operating System
*/
#include <string.h>
#include <core/stdio.h>
#include <core/utility.h>

#include "../mpx_supt.h"
#include "../commands/commands.h"
#include "poll_input.h"
#include "splash.h"

/**
* @brief The command input buffer
*
* This a macro to store the command input buffer  Here we can change
* the ammount of characters we allow to be entered into the command
* handler at once  We currently allow 100 characters 
*/
#define CMDSIZE 100

// Function stubs
int search_commands(char*);

/**
* @brief A struct to hold commands
*
* The COMMAND Struct is a custom struct that is designed to hold custom
* commands 
*
* @param str A string type to hold the name of the command
* @param CommandPointer A pointer to a command so that we can pass commands
*/
typedef struct {
  char* str;
  int (*func)(char*);
} COMMAND;

/**
* @brief Array of COMMANDS that are supported
*/
COMMAND commands[] = {
  {"help", &cmd_help},
  {"version",&cmd_version},
  {"date",&cmd_date},
  {"time", &cmd_time},
  {"clear", &cmd_clear},
  {"quet", &cmd_test_pcb},
  {"blockPCB", &cmd_blockPCB},
  {"resumePCB", &cmd_resume},
  {"suspendPCB", &cmd_suspend},
  {"showPCB", &cmd_show_pcb},
  {"showAllPCBs", &cmd_show_all_pcbs},
  {"showReadyPCBs", &cmd_show_ready_pcbs},
  {"showBlockedPCBs", &cmd_show_blocked_pcbs},
  {"unblockPCB", &cmd_unblock_pcb},
  {"createPCB", &cmd_create_pcb},
  {"deletePCB", &cmd_delete_pcb},
  {"setPriorityPCB", &cmd_set_priority_pcb},
  {NULL, NULL} // leave NULL at the end for searching reasons
};

/********END OF COMMANDS AND FUNCTION DECLARATION*******/

/**
* @brief Entry point for the command handler
*/
int command_handler() {
  // Initialize vars
  char cmd[CMDSIZE];
  int exit = 0;
  int bufSize = CMDSIZE;
  int errCode;
  int cmdidx;

  sys_set_read(&poll_input);

  draw_splash();

  puts("\nType \033[33;1m\"help\"\033[0m for a list of commands\n");

  while (!exit) {
    printf("\n\033[33;1mPOS User Shell\033[34m>>\033[0m ");  // show prompt

    memset(cmd, '\0', CMDSIZE);  // set command as a bunch of \0s
    bufSize = CMDSIZE - 1;       // set bufSize as the command size - 1
    errCode = sys_req(READ, DEFAULT_DEVICE, cmd, &bufSize);

    /******SYS_REQ VALIDATION********/
    if (errCode != 0)  // If there was something wrong with sys_req   
    {
      switch (errCode) {
        case INVALID_OPERATION:
          puts("\033[31;1mInvalid Operation from command handler!\033[0m");
          break;

        case INVALID_BUFFER:
          puts("\033[31;1mInvalid Buffer from command handler!\033[0m");
          break;

        case INVALID_COUNT:
          puts("\033[31;1mInvalid Count from command handler!\033[0m");
          break;
      }

      puts("\033[31;1mFailure to execute command!\033[0m");
      continue;  // move forward with your life!
    }
    /*******END SYS_REQ VALIDATION*******/

    // at this point, we know we have a good command

    // if they have typed in exit    (in the future maybe make a more elegant
    // space-killing solution)
    if ((strcmp(cmd, "exit") == 0) | (strcmp(cmd, "shutdown") == 0))
    {
      char shutdownCmd[10];
      int shutdownBufSize = 10;
      printf("Are you sure that you would like to shutdown? (Y/N): ");
      //read the command and put it into shutdownCmd;
      sys_req(READ, DEFAULT_DEVICE, shutdownCmd, &shutdownBufSize);

      if(tolower(shutdownCmd[0]) == 'y')
      {
      	exit = 1;  // set exit to true
      }
      else
      {
      	puts("\033[32mSystem shutdown canceled!\033[0m");
      }
      continue;	//no need to search commands if we have tried to exit
    }

    cmdidx = search_commands(cmd);

    if (cmdidx == -1)  // if we did not find anything   
    {
      char ret[CMDSIZE + 18];
      sprintf(ret, "\033[31mInvalid Command: %s\033[0m\n", cmd);
      puts(ret);
    } else  // if we did find something   
    {
      (*commands[cmdidx].func)(cmd);
    }
  }

  puts("Exiting prompt! Goodbye and have a \033[1;31ms\033[33mp\033[32ml\033[36me\033[34mn\033[35md\033[31mi\033[33mf\033[32me\033[36mr\033[34mo\033[35mu\033[31ms\033[0m day!\n");

  return 0;
}

/**
* @brief Finds which command in the global COMMANDS array
*
* @param cmd cmd typed by user
*/
int search_commands(char* cmd) {
  int cmdidx;
  unsigned char found;
  char* cmdStart;
  char* testCmd;
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
      cmdidx++;  // increment cmdidx!
    }

    cmd = cmdStart;  // reset cmd back to the beginning
  }

  // if we did not find anything   
  if (commands[cmdidx].str == NULL)
    cmdidx = -1;  // set cmdidx to -1

  return cmdidx;
}
