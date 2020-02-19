/**
* @file cmdBlockPCB.c
*
* @brief Block PCB Command
*/
#include "../pcb/pcb_utils.h"
#include "commandUtils.h"
#include "../pcb/pcb_constants.h"
#include <core/stdio.h>
#include <system.h>

/**
* @brief command to block PCB by name
*
* @returns Success or Failure
*/
int cmd_blockPCB(char* params)
{
  int flag = 0, chk;
  char* pname;
  pcb_t* p = NULL;

  chk = set_flags(params, &flag, 0);

  if(!(flag&NO_FLAG))
  {
    puts("\033[31mNo process Included!\033[0m");
    return FAILURE;
  }

  if (chk != SUCCESS) {
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

  if(p->state == BLOCKED || p->state == SUSPENDED_BLOCKED)
  {
    printf("\033[31mProcess: '%s' already blocked!\033[0m", pname);
    return FAILURE;
  }

  p = remove_pcb(pname);

  if(p->state == READY)
  {
    p->state = BLOCKED;
  } else if (p->state == SUSPENDED_READY)
  {
    p->state = SUSPENDED_BLOCKED;
  }

  insert_pcb(p);
  puts("\033[32mProcess successfully blocked!\033[0m");

  return SUCCESS;
}
