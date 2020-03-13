#include "../pcb/pcb_utils.h"
#include "commandUtils.h"
#include "../pcb/pcb_constants.h"
#include <core/stdio.h>
#include <system.h>

int cmd_suspend(char* params)
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
    printf("\033[31mProcess: '%s' not found!\033[0m\n",pname);
    return FAILURE;
  }

  if(p->process_class == SYSTEM)
  {
    printf("\033[31mProcess not removed. Process '%s' is a SYSTEM process.\033[0m",p->process_name);
    return FAILURE;
  }

  if(p->state == SUSPENDED_BLOCKED || p->state == SUSPENDED_READY)
  {
    printf("\033[31mProcess: '%s' is already suspended!\033[0m\n", pname);
    return FAILURE;
  }

  p = remove_pcb(pname);

  if(p->state == READY)
  {
    p->state = SUSPENDED_READY;
  } else if (p->state == BLOCKED)
  {
    p->state = SUSPENDED_BLOCKED;
  }

  insert_pcb(p);
  puts("\033[32mProcess successfully suspended!\033[0m");

  return SUCCESS;
}