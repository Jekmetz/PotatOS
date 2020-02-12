#include "../mpx_supt.h"
#include "../pcb/pcb_queue.h"
#include "commandUtils.h"
#include "commands.h"

#include <core/stdio.h>
#include <string.h>

/**
 *  FIXME: Remove this file in the future because it is just a test
 *
 *
 *  Here we see the complete behavior of the queue
 */
void chek_state(queue_t* que) {
  printf("(HEAD) %s ", que->head->data->process_name);

  struct node *curr = que->head->next;
  while (curr != NULL)
  {
    printf("-> %s ", curr->data->process_name);
    curr = curr->next;
  }
  puts("");
}

int cmd_test_pcb(char* params) {
  // Init vars
  int flag = 0, chk;

  chk = set_flags(params, &flag, 1, 's', "set");

  if (chk != SUCCESS) {
    puts("\033[31mHouston, we have a problem. Check your flags!\033[0m");
    return FAILURE;
  }

  queue_t* que = construct_queue();

  // Test printed circuit boards //////////////////////////////////////////////
  pcb_t pcb[10];

  for (int i = 0; i < (int)(sizeof(pcb) / sizeof(pcb_t)); i++) {
    pcb[i].priority = i + 3;
    pcb[i].process_class = SYSTEM;
    pcb[i].process_name = sys_alloc_mem(sizeof(char) * 16);
    sprintf(pcb[i].process_name, "PROC:%02d", i);
    printf("Creating process : %s\n", pcb[i].process_name);
    pcb[i].state = READY;

    enqueue(que, &pcb[i]);
    printf("Enqued %d\n", i);
    chek_state(que);
  }
  /////////////////////////////////////////////////////////////////////////////

  pcb_t* deq = dequeue(que);
  if (deq == &(pcb[0])) {
    puts("\033[1;32mSuccessfully dequed\033[0m");
  } else {
    puts("\033[31mFailed to dequeue\033[0m");
  }
  chek_state(que);

  /*if (remove_pcb("PROC:02")) {
    puts("Removed proc 2");
  } else {
    puts("did not remove 2");
  }*/
  chek_state(que);

  enqueue(que, &(pcb[1]));
  puts("Enqueued pcb 1");
  chek_state(que);

  // FIXME: This line below causes a seg fault
  destruct_queue(que);
  puts("\033[1;32mCompleted test of queue\033[0m");

  return 1;
}