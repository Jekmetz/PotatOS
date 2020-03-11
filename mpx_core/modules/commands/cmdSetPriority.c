#include <core/stdio.h>
#include <system.h>
#include <string.h>
#include "../pcb/pcb_constants.h"
#include "../pcb/pcb_utils.h"
#include "commandUtils.h" 

int cmd_set_priority_pcb(char* params){
  // Variables
  int flag = 0, chk;
  char* process_name;
  int process_priority;
  pcb_t* p = NULL;

  // Calling set flags
  chk = set_flags(params, &flag, 1,
    'p',"priority"
    );

  // If they did not use no flag to declare process
  if(!(flag&NO_FLAG))
  {
    puts("\033[31mNo process Included!\033[0m");
    return FAILURE;
  }

  // If set flags fails
  if (chk != SUCCESS) {
    puts("\033[31mHouston, we have a problem. Check your flags!\033[0m");
    return FAILURE;
  }

  // If P flag not used
  if(!(flag & P_FLAG))
  {
    puts("\033[31mNo process priority specified! Please use '[-p|--priority] <priority>' to specify a process priority!\033[0m");
    return FAILURE;
  }

  // Getting the proccess name
  process_name = get_pvalue('\0');

  // Getting the proces priority
  process_priority = atoi(get_pvalue('p'));

  //find that boi
  p = find_pcb(process_name);
  if(p != NULL && p->process_class == SYSTEM)
  {
    printf("\033[31mProcess priority not set. Process '%s' is a SYSTEM process.\033[0m",p->process_name);
    return FAILURE;
  }

  // Attempting to remove the proces, remove_pcb() handles sys_free_mem for us
  p = remove_pcb(process_name);

  // If process equals NULL, it did not return anything therefor that process
  // does not exist
  if(p == NULL)
  {
    printf("\033[31mProcess: '%s' not found!\033[0m\n",process_name);
    return FAILURE;
  }

  // If we made it here, the process return and we can change the priority
  p->priority = process_priority;
  
  // Inserting the pcb back, insert handles insertion per priority
  insert_pcb(p);

  printf("Process %s priority successfully changed to %d\n", process_name, process_priority);

  return SUCCESS;
}
