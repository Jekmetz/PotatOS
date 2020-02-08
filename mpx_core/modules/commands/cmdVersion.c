/**
* @file cmdVersion.c
*
* @brief Version Command
*/

#include <string.h>
#include <core/stdio.h>
#include <core/utility.h>
#include <core/io.h>
#include <stdarg.h>

#include "commands.h"
#include "time.h"
#include "../mpx_supt.h"
#include "commandUtils.h"

/**
* @brief Macro to hold the short version
*/
#define VERSION \
"\
Version 1.0\
"

/**
* @brief Macro to hold the full version
*/
#define VERSION_FULL \
"\
Version 1.0\n\
Module one\n\
Developers:\n\
  Hasan Ibraheem\n\
  Henry Vos\n\
  Jay Kmetz\n\
  Nicholas Fryer\
"

/**
* @brief The version command will show the version information
*
* The version command can be called to display the version information.
* The shortened return will just show the short version.
* The long return will include the current module, the version, and the
* contributing developers
*
* @param params param string typed by user
*
* @return A version page
*/
int cmd_version(char* params) {
  // Init vars
  int flag = 0, chk;

  chk = set_flags(params, &flag, 1,
    'f',"full"
  );

  if (chk != SUCCESS) {
    puts("\033[31mHouston, we have a problem. Check your flags!\033[0m");
    return FAILURE;
  }

  // char* cmd = get_pvalue('f');

  //get flag values
  // If no full flag set
  if( !(flag & F_FLAG)){
	   printf(VERSION);
  }
  // If full flag used
  else{
    printf(VERSION_FULL);
  }

  return SUCCESS;
}