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
  pcb_t* p;

  chk = set_flags(params, &flag, 1,
    'p',"pname"
    );

  if (chk != SUCCESS) {
    puts("\033[31mHouston, we have a problem. Check your flags!\033[0m");
    return FAILURE;
  }

  if(!(flag & P_FLAG))
  {
  	puts("\033[31mNo process name specified! Please use '[-p|--pname] <pname>' to specify a process name!\033[0m");
  	return FAILURE;
  }

  pname = get_pvalue('p');
  p = remove_pcb(pname);

  if(p == NULL)
  {
  	printf("\033[31Process: '%s' not found!\033[0m",pname);
  	return FAILURE;
  }

  p->state = BLOCKED;
  insert_pcb(p);
  puts("\033[32mProcess successfully blocked!\033[0m");

  return SUCCESS;
}
