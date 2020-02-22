#include <core/stdio.h>
#include <system.h>

#include "../pcb/pcb_constants.h"
#include "../pcb/pcb_utils.h"
#include "commandUtils.h"
#include "commands.h"

int cmd_show_ready_pcbs(char* params) {
  if (params) {
    // Ignore unused parameter warning
  }

  int total_printed = 0;

  queue_t* all_queues[] = {
      get_ready_queue(),
      get_suspended_ready_queue(),
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
