#include "../mpx_supt.h"
#include "pcb_constants.h"
#include "pcb_queue.h"
#include <string.h>

queue_t *ready_queue, *blocked_queue, *suspended_r_queue, *suspended_b_queue;

void init_queue() {
  ready_queue = construct_queue();
  blocked_queue = construct_queue();
  suspended_r_queue = construct_queue();
  suspended_b_queue = construct_queue();
}

/*
  TODO: 
  pcb_t* allocate_pcb();
  int setup_pcb(char*, PROCESS_CLASS, int priority);
  int free_pcb(pcb_t*);
  int insert_pcb(pcb_t*);
*/

pcb_t* find_pcb(char* name) {
  queue_t* currs[4] = {ready_queue, blocked_queue, suspended_r_queue,
                       suspended_b_queue};
  for (int i = 0; i < (int)(sizeof(currs) / sizeof(queue_t)); i++) {
    struct node* curr = currs[i]->head;
    while (curr != NULL) {
      // checking if the process is the correct one
      if (strcmp(curr->data->process_name, name) == 0) {
        return curr->data;
      }
      curr = curr->next;
    }
  }
  return NULL;
}

int remove_pcb(char* name) {
  queue_t* currs[4] = {ready_queue, blocked_queue, suspended_r_queue,
                       suspended_b_queue};
  for (int i = 0; i < (int)(sizeof(currs) / sizeof(queue_t)); i++) {
    struct node* curr = currs[i]->head;
    while (curr != NULL) {
      // checking if the process is the correct one
      if (strcmp(curr->data->process_name, name) == 0) {
        // destroying the process
        ((struct node*)curr->prev)->next = curr->next;
        ((struct node*)curr->next)->prev = curr->prev;
        sys_free_mem(curr);
        return 1;
      }
      curr = curr->next;
    }
  }
  return 0;
}