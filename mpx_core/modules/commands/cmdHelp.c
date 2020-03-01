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

void print_help_menu();

// Note on the macro help pages: You can literal tab to insert tabs, newlines must use \n,
// all lines must end with \ due to macro requirments

/**
* @brief Macro to hold the help page for command help
*/
#define HELP \
"\
Description:\n\
  Request a help page for a command\n\n\
Usage:\n\
  help <command>\n\n\
Flags:\n\
  None\n\n\
Example:\n\
  help time\n\
  help version\n\
"

/**
* @brief Macro to hold the help page for command version
*/
#define VERSION \
"\
Description:\n\
  View the version information of the current PotatOS\n\n\
Usage:\n\
  version [-f | --full]\n\n\
Flags:\n\
  [-f | --full] - Show entire verion\n\n\
Example:\n\
  version\n\
  version --full\n\
"

/**
* @brief Macro to holdf the help page for command shutdown
*/
#define SHUTDOWN \
"\
Description:\n\
  Shutdown the PotatOS\n\n\
Usage:\n\
  shutdown\n\n\
Flags:\n\
  None\n\n\
Notes:\n\
  Must confirm with Yes before shutdown\n\
"

/**
* @brief Macro to hold the help page for command date
*/
#define DATE \
"\
Description:\n\
  See/set the system date\n\n\
Usage:\n\
  date [-s | --set]\n\n\
Flags:\n\
  [-s | --set] - Set the date in DD/MM/YYYY\n\
    Where all values are integers\n\n\
Example:\n\
  date -s 08/24/1994\n\
  date --set 01/01/2019\n\
"

/**
* @brief Macro to hold the help page for command time
*/
#define TIME \
"\
Description\n\
  See/set the system time\n\n\
Usage:\n\
  time [-s | --set]\n\n\
Flags:\n\
  [-s | --set] - Set the time in HH:MM:SS\n\
    Where all values are integers and using 24 hour time\n\n\
Example:\n\
  time -s 12:24:32\n\
  time --set 16:02:00\n\
"

/**
* @brief Macro to hold the help page for command blockPCB
*/
#define BLOCKPCB \
"\
Description\n\
  Block a process by name\n\n\
Usage:\n\
  blockPCB <proccess_name>\n\n\
Example:\n\
  blockPCB PROC1\n\
"

/**
* @brief Macro to hold the help page for command blockPCB
*/
#define RESUMEPCB \
"\
Description\n\
  Resume a process by name\n\n\
Usage:\n\
  resumePCB <proccess_name>\n\n\
Example:\n\
  resumePCB PROC1\n\
"

/**
* @brief Macro to hold the help page for command blockPCB
*/
#define SUSPENDPCB \
"\
Description\n\
  Suspend a process by name\n\n\
Usage:\n\
  suspendPCB <proccess_name>\n\n\
Example:\n\
  suspendPCB PROC1\n\
"

/**
* @brief Macro to hold the help page for command showPCB
*/
#define SHOWPCB \
  "\
  Description:\n\
    Show a PCB by name\n\n\
  Usage:\n\
    showPCB <proccess_name>\n\n\
  Example:\n\
    showPCB PROC1\n\
  "

/**
* @brief Macro to hold the help page for command showAllPCBs
*/
#define SHOWALLPCBS \
"\
Description\n\
  Show all the PCBs\n\n\
Usage:\n\
  showAllPCBs\n\
"

/**
* @brief Macro to hold the help page for command showReadyPCBs
*/
#define SHOWREADYPCBS \
"\
Description\n\
  Show all the ready PCBs\n\n\
Usage:\n\
  showReadyPCBs\n\
"

/**
* @brief Macro to hold the help page for command showBlockedPCBs
*/
#define SHOWBLOCKEDPCBS \
"\
Description\n\
  Show all the blocked processes\n\n\
Usage:\n\
  showBlockedPCBs\n\
"

#define CREATEPCB \
"\
Description:\n\
  Creates a PCB and adds it to queue\n\n\
Usage:\n\
  createPCB <process_name> {[-p | --priority] <integer_priority=BIG_PI>} {[-c | --class][app | sys]}\n\n\
Examples:\n\
  createPCB abc\n\
  createPCB def -p 4\n\
  createPCB ghi --class sys\n\
"

#define UNBLOCKPCB \
"\
Description:\n\
  Moves a PCB from blocked to an unblocked state\n\n\
Usage:\n\
  unblockPCB <process_name>\n\n\
Example:\n\
  unblockPCB abc\n\
"

#define SETPRIORITYPCB \
"\
Description:\n\
  Changes an existing PCB's priority to a new priority\n\n\
Usage:\n\
  setPriorityPCB <process_name> [-p | --priority] <new priority>\n\n\
Example:\n\
  setPriorityPCB testPCB -p 7\n\
"

#define DELETEPCB \
"\
Description:\n\
  Delete an existing PCB\n\n\
Usage:\n\
  deletePCB <process_name>\n\n\
Example:\n\
  deletePCB testPCB\n\
"

#define POTAT \
"\
Description:\n\
  Instantly brighten your day\n\n\
Usage:\n\
  potat\n\n\
Example:\n\
  potat\n\
"

/**
* @brief A struct to hold help outputs
*
* The COMMAND Struct is a custom struct that is designed to hold custom
* commands
*
* @param str A string type to hold the name of the command
* @param command_help_page A string that holds the actual help page
*/
typedef struct {
  char* command_name;
  char* command_help_page;
} HELP_PAGES;


/**
* @brief Array of COMMANDS that are supported
*/
HELP_PAGES help_pages[] = {
  {"help", HELP},
  {"version", VERSION},
  {"shutdown", SHUTDOWN},
  {"date", DATE},
  {"time", TIME},
  {"blockPCB", BLOCKPCB},
  {"resumePCB", RESUMEPCB},
  {"suspendPCB", SUSPENDPCB},
  {"showPCB", SHOWPCB},
  {"showAllPCBs", SHOWALLPCBS},
  {"showReadyPCBs", SHOWREADYPCBS},
  {"showBlockedPCBs", SHOWBLOCKEDPCBS},
  {"createPCB", CREATEPCB},
  {"unblockPCB", UNBLOCKPCB},
  {"setPriorityPCB", SETPRIORITYPCB},
  {"deletePCB", DELETEPCB},
  {"potat", POTAT},
  {NULL, NULL} // leave NULL at the end for searching reasons
};

int cmd_help(char* params) {
  // Init vars
  int flag = 0, chk, check,i;
  char *cmd;

  // trim and set flags
  chk = set_flags(trim(params),&flag,0);

  if(chk != SUCCESS)
  {
    puts("\033[31mHouston, we have a problem. Check your flags!\033[0m");
    return FAILURE;
  }

  //if there was nothing specified...
  if(!(flag & NO_FLAG))
  {
    print_help_menu();
    return SUCCESS;
  }
  // If there is a requested command, get it from the NO_VALUE using \0
  cmd = get_pvalue('\0');

  i = 0; // Setting counter to zero
  check = 0; // Setting check to zero, false
  while(help_pages[i].command_name != NULL){
    // If the command is found
    if(strcmp(help_pages[i].command_name,cmd) == 0){
      // Printing the help page
      printf(help_pages[i].command_help_page);
      // Setting check, or found, to one, true
      check = 1;
    }
    // Incrementing the counter
    i++;
  }
  // If check was not found
  if (check == 0){
    printf("\033[31mCommand \"%s\" not found.\033[0m\n",cmd);
 }

  return SUCCESS;  // successful response
}

void print_help_menu()
{
  int idx = 0;
  puts("You can request a help page for the following commands using help <cmd name>\n");
  while(help_pages[idx].command_name != NULL) //while we have more to go...
  {
    puts(help_pages[idx].command_name);
    idx++;
  }
}