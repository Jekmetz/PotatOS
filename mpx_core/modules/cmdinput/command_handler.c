/**
* @file command_handler c
*
* @brief The primary command handler for the Operating System
*/
#include <string.h>
#include <system.h>
#include <core/stdio.h>
#include <core/utility.h>

#include "../mpx_supt.h"
#include "../commands/commands.h"
#include "../commands/commandUtils.h"
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


/********END OF COMMANDS AND FUNCTION DECLARATION*******/

/**
* @brief Entry point for the command handler
*/
void command_handler() {
  // Initialize vars
  char cmd[CMDSIZE];
  int exit = 0;
  int bufSize = CMDSIZE;
  int errCode;
  COMMAND* cmdPtr;

  sys_set_read(&poll_input);

  puts("\n\"When a process \033[1;31mends\033[0m, it says 'I'm done... \033[3;31mKill me.\033[0m'\" - Alexander Wilson, 2020\n");

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
    if ((strcmp(trim(cmd), "exit") == 0) | (strcmp(trim(cmd), "shutdown") == 0))
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

    cmdPtr = search_commands(cmd);

    if (cmdPtr == NULL)  // if we did not find anything   
    {
      char ret[CMDSIZE + 18];
      sprintf(ret, "\033[31mInvalid Command: %s\033[0m\n", cmd);
      puts(ret);
    } else  // if we did find something   
    {
      (*cmdPtr->func)(cmd);
    }

    sys_req(IDLE, DEFAULT_DEVICE, NULL, NULL);
  }

  puts("Exiting prompt! Goodbye and have a \033[1;31ms\033[33mp\033[32ml\033[36me\033[34mn\033[35md\033[31mi\033[33mf\033[32me\033[36mr\033[34mo\033[35mu\033[31ms\033[0m day!\n");

  return 0;
}
