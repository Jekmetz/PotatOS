#include <core/stdio.h>
#include <system.h>

#include "../pcb/pcb_constants.h"
#include "../pcb/pcb_utils.h"
#include "commandUtils.h"
#include "commands.h"

int cmd_show_blocked_pcbs(char* params) {
  
  int flag = 0, chk;

  chk = set_flags(params, &flag, 0);

  if(chk != SUCCESS)
  {
    puts("\033[31mHouston, we have a problem. Check your flags!\033[0m");
    return FAILURE; 
  }

  int total_printed = 0;

  queue_t* all_queues[] = {
      get_blocked_queue(),
      get_suspended_blocked_queue(),
  };

  for (int i = 0; i < 2; i++) {
    node_t* curr = all_queues[i]->head;

    while (curr != NULL) {
      print_pcb_info(curr->data);
      puts("");
      curr = curr->next;
      total_printed++;
    }
  }

  printf("Total printed: %i\n", total_printed);

  return SUCCESS;
}
