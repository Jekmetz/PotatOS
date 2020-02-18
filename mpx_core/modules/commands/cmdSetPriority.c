#include <core/stdio.h>
#include <system.h>
#include "../pcb/pcb_constants.h"
#include "../pcb/pcb_utils.h"
#include "commandUtils.h"

/**
* @brief command to set PCB priority
*
* @returns Success if the PCB priority was updated, failure for anything else
*/
int cmd_SetPriority(char* params, int* priority){
  // Variables
  int flag = 0, chk;
  char* process_name;
  int* process_priority;
  pcb_t* p = NULL;

  // Calling set flags
  chk = set_flags(params, &flag, 1,
    'p',"pname",
    'r','priority'
    );

  // If set flags fails
  if (chk != SUCCESS) {
    puts("\033[31mHouston, we have a problem. Check your flags!\033[0m");
    return FAILURE;
  }

  // If p flag not used
  if(!(flag & P_FLAG))
  {
    puts("\033[31mNo process name specified! Please use '[-p|--pname] <process name>' to specify a process name!\033[0m");
    return FAILURE;
  }

  // If r flag not used
  if(!(flag & R_FLAG))
  {
    puts("\033[31mNo process priority specified! Please use '[-r|--priority] <priority>' to specify a process priority!\033[0m");
    return FAILURE;
  }

  // Getting the proccess name
  process_name = get_pvalue('p');
  // Getting the proces priority
  process_priority = get_pvalue('r');
  // Attempting to remove the proces, remove_pcb() handles sys_free_mem for us
  process = remove_pcb(process_name);

  // If process equals NULL, it did not return anything therefor that process
  // does not exist
  if(process == NULL)
  {
    printf("\033[31Process: '%s' not found!\033[0m",process_name);
    return FAILURE;
  }

  // If we made it here, the process return and we can change the priority
  p->priority = process_priority;
  // Inserting the pcb back, insert handles insertion per priority
  insert_pcb(process);

  printf("Process %s priority succesfully changed to %d", process_name, process_priority);

  return SUCCESS;
}
