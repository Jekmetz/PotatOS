#include "../mpx_supt.h"
#include "pcb_constants.h"
#include "pcb_queue.h"
#include <string.h>

queue_t *ready_queue, *blocked_queue, *suspended_r_queue, *suspended_b_queue;
pcb_t *running;

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
*/

int insert_pcb(pcb_t *proc)
{
  switch (proc->state)
  {
    case RUNNING:
      priority_enqueue(ready_queue, running);
      running = proc;
      break;
    case READY:
      priority_enqueue(ready_queue, proc);
      break;
    case BLOCKED:
      enqueue(blocked_queue, proc);
      break;
    case SUSPENDED_READY:
      priority_enqueue(suspended_r_queue, proc);
      break;
    case SUSPENDED_BLOCKED:
      enqueue(suspended_b_queue, proc);
      break;
    default:
      return 0;
  }
  return 1;
}

/**
 * @brief Finds a PCB in all queues.
 * 
 * Searches through all the system PCB queues to find a PCB with the specified
 * process name given by pname.
 * 
 * @param pname The name of the process you want to find the PCB of.
 * @return A pointer to the pcb with the specified name or 'NULL' for not found.
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

/**
 * @brief Removes a PCB by process name.
 * 
 * Searches through all system queues to find the PCB with the given name.
 * 
 * @param pname Name of the process you want to remove.
 * @return Success condition (boolean).
 */
pcb_t *remove_pcb(char* name) {
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
        pcb_t *ret = curr->data;
        sys_free_mem(curr);
        return ret;
      }
      curr = curr->next;
    }
  }
  return NULL;
}