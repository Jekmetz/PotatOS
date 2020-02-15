/**
* @file cmdhelp.c
*
* @brief Help command
*/

#include <string.h>
#include <core/stdio.h>
#include <core/utility.h>
#include <core/io.h>
#include <stdarg.h>

#include "commands.h"
#include "commandUtils.h"
#include "time.h"
#include "../mpx_supt.h"

// Note on the macro help pages: You can literal tab to insert tabs, newlines must use \n,
// all lines must end with \ due to macro requirments

/**
* @brief Macro to hold the list of commands that have help pages
*/
#define HELP_MENU \
"\
You can request a help page for the following commands \
using help <cmd name>\n\
  help\n\
  version\n\
  shutdown\n\
  date\n\
  time\n\
  blockPCB\n\
  suspendPCB\n\
  resumePCB\n\
"

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
  help version\
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
  version --full\
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
  Must confirm with Yes before shutdown\
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
  date --set 01/01/2019\
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
  time --set 16:02:00\
"

/**
* @brief Macro to hold the help page for command blockPCB
*/
#define BLOCKPCB \
"\
Description\n\
  Block a process by name\n\n\
Usage:\n\
  blockPCB [-p | --pname] <proccess_name>\n\n\
Example:\n\
  blockPCB -p PROC1\n\
  blockPCB --pname PROC2\
"

/**
* @brief Macro to hold the help page for command blockPCB
*/
#define RESUMEPCB \
"\
Description\n\
  Resume a process by name\n\n\
Usage:\n\
  resumePCB [-p | --pname] <proccess_name>\n\n\
Example:\n\
  resumePCB -p PROC1\n\
  resumePCB --pname PROC2\
"

/**
* @brief Macro to hold the help page for command blockPCB
*/
#define SUSPENDPCB \
"\
Description\n\
  Suspend a process by name\n\n\
Usage:\n\
  suspendPCB [-p | --pname] <proccess_name>\n\n\
Example:\n\
  suspendPCB -p PROC1\n\
  suspendPCB --pname PROC2\
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
  {NULL, NULL} // leave NULL at the end for searching reasons
};

/**
* @brief The help command will show a page to assist users with commands
*
* The help command can be called to do one of two things
*   List all the commands that have help pages
*   Request a help page for a certain command
*
*
* @param params param string typed by user
*
* @return A help page
*/
int cmd_help(char* params) {
  // Init vars
  int flag = 0, chk, check,i;
  char *cmd;
  char buf[100];

  // Trimming the command
  sprintf(buf,"%s",trim(params));

  chk = set_flags(buf,&flag,0);

  if(chk != SUCCESS)
  {
    puts("\033[31mHouston, we have a problem. Check your flags!\033[0m");
    return FAILURE; 
  }

  //if there was nothing specified...
  if(!(flag & NO_FLAG))
  {
    printf(HELP_MENU);
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
    printf("\033[31mCommand \"%s\" not found.\033[0m",cmd);
 }

  return SUCCESS;  // successful response
}