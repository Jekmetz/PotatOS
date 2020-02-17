/**
 * @file cmdShowAllPCBs.c
 *
 * @brief Show All PCBs command
 */

#include <core/stdio.h>
#include <system.h>

#include "../pcb/pcb_constants.h"
#include "../pcb/pcb_utils.h"
#include "commandUtils.h"
#include "commands.h"

// Again, plan to remove when all is well and done
pcb_t test_pcb1 = {
    .process_name = "test",
    .process_class = SYSTEM,
    .priority = 1,
    .state = READY,
    .suspended = 0,
    .stack = {0},
};

int cmd_show_all_pcbs(char* params) {
  if (params) {
    // Ignore unused parameter warning
  }

  int total_printed = 0;

  queue_t* all_queues[] = {
      get_ready_queue(),
      get_suspended_ready_queue(),
      get_blocked_queue(),
      get_suspended_blocked_queue(),
  };

  for (int i = 0; i < 4; i++) {
    node_t* curr = all_queues[i]->head;

    while (curr != NULL) {
      print_pcb_info(curr->data);
      puts("");
      curr = curr->next;
    }
  }

  print_pcb_info(&test_pcb1);
  puts("");

  printf("Total printed: %i\n", total_printed);

  return SUCCESS;
}
