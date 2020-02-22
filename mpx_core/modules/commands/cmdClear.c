#include <string.h>
#include <core/stdio.h>
#include <core/utility.h>
#include <core/io.h>
#include <stdarg.h>

#include "commands.h"
#include "time.h"
#include "../mpx_supt.h"
#include "commandUtils.h"

int cmd_clear(char* params)
{
  //Unused... supresses warning
  if(params){}

  puts("\033[2J\033[H");

  return SUCCESS;
}

