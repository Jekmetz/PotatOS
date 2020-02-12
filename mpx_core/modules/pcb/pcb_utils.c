#include "pcb_queue.h"
#include "pcb_constants.h"

queue_t *ready_queue, *blocked_queue, *suspended_r_queue, *suspended_b_queue;


void init_queue(){
	ready_queue = construct_queue();
	blocked_queue = construct_queue();
	suspended_r_queue = construct_queue();
	suspended_b_queue = construct_queue();
}

/**
 *  FIXME: This is not merciless enough
 */
int remove_pcb(char* name) {
  if(name) {}
  /*
  * TODO: Search through all of the queues for the name,
  * find correct pcb, kill it and remove it. Look at slides
  * for details!
  */

  // setting the moving pointer to start at head
  /*struct node* curr = que->head;
  while (curr != NULL) {
    // checking if the process is the correct one
    if (strcmp(curr->data->process_name, name) == 0) {
      // destroying the process
      ((struct node*)curr->prev)->next = curr->next;
      ((struct node*)curr->next)->prev = curr->prev;
      sys_free_mem(curr);
      que->size--;
      return SUCCESS;
    }
    curr = curr->next;
  }*/
  return 0;
}