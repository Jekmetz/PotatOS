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
  pcb_t* p;

  chk = set_flags(params, &flag, 1,
    'p','pname');

  if(chk != SUCCESS)
  {
    puts("\033[31mHouston, we have a problem. Check your flags!\033[0m");
    return FAILURE; 
  }

  if(!(flag & P_FLAG))
  {
  	puts("\033[31mNo Process name specified! Use [-p | --pname] <proc_name> to specify process!\033[0m");
  	return FAILURE;
  }

  pname = get_pvalue('p');
  p = remove_pcb(pname);

  if(p == NULL)
  {
  	printf("\033[31mNo proccess found with name: '%s'\033[0m",pname);
  	return FAILURE;
  }

  //If we have a valid process at this point...
  if(p->state == PROCESS_STATE.SUSPENDED_BLOCKED)
  	p->state = PROCESS_STATE.BLOCKED;
  else if (p->state == PROCESS_STATE.SUSPENDED_READY)
  	p->state = PROCESS_STATE.READY;

  insert_pcb(p);
  return SUCCESS;
}