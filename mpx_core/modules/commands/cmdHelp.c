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
  blockPCB\
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
  char* cmdStart;
  char* request_help_page;
  int i, check;
  char buf[100];

  // Triming the command
  sprintf(buf,"%s",trim(params));

  // The start of the help command
  cmdStart = buf;

  // Jumping forward 4 spaces
  cmdStart = cmdStart + 4;

  // If its just help without any other arguments
  if(*cmdStart++ == NULL)
  {
    printf(HELP_MENU);
  }
  // If there is a requested command
  else
  {
    // Skipping white space between "help" and requested command
    while(isspace(cmdStart)){
      cmdStart++;
    }

    // request_help_page will have the requested help page after help
    request_help_page = cmdStart;

    i = 0; // Setting counter to zero
    check = 0; // Setting check to zero, false
    while(help_pages[i].command_name != NULL){
      // If the command is found
      if(strcmp(help_pages[i].command_name,request_help_page) == 0){
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
      printf("Command \"%s\" not found.",request_help_page);
    }
  }

  return SUCCESS;  // successful response
  }