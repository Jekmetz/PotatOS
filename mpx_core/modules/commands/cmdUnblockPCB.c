#include <string.h>
#include <core/stdio.h>
#include "../pcb/pcb_constants.h"
#include "commandUtils.h"
#include "commands.h"
#include "../mpx_supt.h"
#include "../pcb/pcb_utils.h"
#include "../pcb/pcb_constants.h"

int cmd_unblock_pcb(char* params) {
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

  if(p->state == READY || p->state == SUSPENDED_READY)
  {
    printf("\033[31mProcess: '%s' not blocked!\033[0m", pname);
    return FAILURE;
  }

  p = remove_pcb(pname);

  if(p == NULL)
  {
    printf("\033[31mProcess: '%s' not found!\033[0m",pname);
    return FAILURE;
  }

  if(p->state == BLOCKED)
  {
    p->state = READY;
  } else if (p->state == SUSPENDED_BLOCKED)
  {
    p->state = SUSPENDED_READY;
  }
  insert_pcb(p);
  puts("\033[32mProcess successfully unblocked!\033[0m");

  return SUCCESS;
}
