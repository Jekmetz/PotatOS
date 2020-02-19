/**
* @file cmdResume.c
*
* @brief Resume PCB Command
*/
#include "../pcb/pcb_utils.h"
#include "commandUtils.h"
#include "../pcb/pcb_constants.h"
#include <core/stdio.h>
#include <system.h>

/**
* @brief Resume PCB command
*
* @return SUCCESS or FAILURE
*/
int cmd_resume(char* params)
{
  //INIT VARS
  int flag,chk;
  char* pname;
  pcb_t* p = NULL;

  chk = set_flags(params, &flag, 0);

  if(chk != SUCCESS)
  {
    puts("\033[31mHouston, we have a problem. Check your flags!\033[0m");
    return FAILURE; 
  }

  if(!(flag&NO_FLAG))
  {
    puts("\033[31mHouston, we have a problem. Check your flags!\033[0m");
    return FAILURE;
  }

  pname = get_pvalue('\0');
  p = find_pcb(pname);

  if(p == NULL)
  {
    printf("\033[31mProcess: '%s' not found!\033[0m",pname);
    return FAILURE;
  }

  if(p->state == BLOCKED || p->state == READY || p->state == RUNNING)
  {
    printf("\033[31mProcess: '%s' is not suspended!\033[0m", pname);
    return FAILURE;
  }

  p = remove_pcb(pname);

  if(p->state == SUSPENDED_READY)
  {
    p->state = READY;
  } else if (p->state == SUSPENDED_BLOCKED)
  {
    p->state = BLOCKED;
  }

  insert_pcb(p);
  puts("\033[32mProcess successfully resumed!\033[0m");

  return SUCCESS;
}