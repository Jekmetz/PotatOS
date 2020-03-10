#include <core/stdio.h>
#include <system.h>
#include "../pcb/pcb_constants.h"
#include "../pcb/pcb_utils.h"
#include "commandUtils.h"

int cmd_delete_pcb(char* params){
  // Variables
  int flag = 0, chk;
  char* process_name;
  pcb_t* p = NULL;

  // Calling set flags
  chk = set_flags(params, &flag, 0);

  // If set flags fails
  if (chk != SUCCESS) {
    puts("\033[31mHouston, we have a problem. Check your flags!\033[0m");
    return FAILURE;
  }

  // If p flag not used
  if(!(flag & NO_FLAG))
  {
    puts("\033[31mNo process name specified! Please use '[-p|--pname] <process name>' to specify a process name!\033[0m");
    return FAILURE;
  }

  // Getting the proccess name from the pvalue
  process_name = get_pvalue('\0');

  //find process
  p = find_pcb(process_name);
  if(p != NULL && p->process_class == SYSTEM)
  {
    printf("\033[31Process not removed. Process '%s' is a SYSTEM process.\033[0m",p->process_name);
    return FAILURE;
  }

  // Attempting to remove the proces, remove_pcb() handles sys_free_mem for us
  p = remove_pcb(process_name);

  // If process equals NULL, it did not return anything therefor that process
  // does not exist
  if(p == NULL)
  {
    printf("\033[31Process: '%s' not found!\033[0m",process_name);
    return FAILURE;
  }

  free_pcb(p);
  printf("PCB %s succesfully removed\n",process_name);
  // If we made it here, the process return and we can return success
  return SUCCESS;
}
