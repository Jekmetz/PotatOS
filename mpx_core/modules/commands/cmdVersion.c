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
Version 3.5\n\
"

/**
* @brief Macro to hold the full version
*/
#define VERSION_FULL \
"\
Version 3.5\n\
Module 3/4\n\
Developers:\n\
  Hasan Ibraheem\n\
  Henry Vos\n\
  Jay Kmetz\n\
  Nicholas Fryer\n\
"

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