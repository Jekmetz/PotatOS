#include "../mpx_supt.h"
#include "pcb_constants.h"
#include "pcb_queue.h"
#include "pcb_utils.h"
#include <string.h>
#include <core/stdio.h>

#define NUMQUEUES 4

queue_t *ready_queue, *blocked_queue, *suspended_r_queue, *suspended_b_queue;
pcb_t *running;

void init_queue() {
  ready_queue = construct_queue();
  blocked_queue = construct_queue();
  suspended_r_queue = construct_queue();
  suspended_b_queue = construct_queue();
}


/**
* @brief simply allocates space for a pcb and returns that pointer
*
* @return pointer to allocated pcb
*/
pcb_t* allocate_pcb()
{
  return (pcb_t*) sys_alloc_mem(sizeof(pcb_t));
}

/**
* @brief frees the space for a pcb
*
* @return Success or failure
*/
// Frees a pcb
int free_pcb(pcb_t* p)
{
  return sys_free_mem(p);
}

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
* @brief command to setup new PCB
*
* @returns Success if the PCB was created, failure for anything else
*/
pcb_t* setup_pcb(char* pname, PROCESS_CLASS pclass, int priority){
  // Allocating new PCB
  pcb_t* pcb = allocate_pcb();

  // If allocate fails, returns null
  if(pcb == NULL){
    printf("\033[31Process: '%s' failed to setup!\033[0m",pname);
    return NULL;
  }

  // Setting values in pcb
  pcb->process_name = pname;
  pcb->process_class = pclass;
  pcb->priority = priority;
  pcb->state = READY;
  pcb->stacktop = (unsigned char*)(pcb->stack + 2048 - sizeof(context_t));

  return pcb;
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
  queue_t* currs[NUMQUEUES] = {ready_queue, blocked_queue, suspended_r_queue,
                       suspended_b_queue};
  for (int i = 0; i < NUMQUEUES; i++) {
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
  queue_t* currs[NUMQUEUES] = {ready_queue, blocked_queue, suspended_r_queue,
                       suspended_b_queue};
  for (int i = 0; i < NUMQUEUES; i++) {
    //Handle edge case of the first boi
    if(currs[i]->head != NULL && strcmp(currs[i]->head->data->process_name,name) == 0)
    {
      pcb_t *ret = currs[i]->head->data;
      currs[i]->head = currs[i]->head->next;
      currs[i]->head->prev = NULL;
      (currs[i]->size)--;
      return ret;
    }

    struct node* curr = currs[i]->head;

    //Do the rest
    while (curr != NULL) {
      // checking if the process is the correct one
      if (strcmp(curr->data->process_name, name) == 0) {
        // destroying the process
        ((struct node*)curr->prev)->next = curr->next;
        ((struct node*)curr->next)->prev = curr->prev;
        pcb_t *ret = curr->data;
        sys_free_mem(curr);
        (currs[i]->size)--;
        return ret;
      }
      curr = curr->next;
    }
  }

  return NULL;
}

queue_t* get_ready_queue() {
  return ready_queue;
}

queue_t* get_blocked_queue() {
  return blocked_queue;
}

queue_t* get_suspended_ready_queue() {
  return suspended_r_queue;
}

queue_t* get_suspended_blocked_queue() {
  return suspended_b_queue;
}

pcb_t** get_running_process() {
  return running == NULL ? NULL : &running;
}

void print_pcb_info(const pcb_t* pcb) {
  // TODO: possible make a boolean to string function
  char* suspended;

  if (pcb->state == SUSPENDED_BLOCKED || pcb->state == SUSPENDED_READY) {
    suspended = "true";
  } else {
    suspended = "false";
  }

  printf(
      "Name: \033[32m%s\033[37m\n"
      "Process Class: \033[32m%s\033[37m\n"
      "State: \033[32m%s\033[37m\n"
      "Priority: \033[32m%i\033[37m\n"
      "Suspended: \033[32m%s\033[37m\n",

      pcb->process_name,
      get_process_class_string(pcb->process_class),
      get_process_state_string(pcb->state),
      pcb->priority,
      suspended
  );
}

const char* get_process_class_string(PROCESS_CLASS process_class) {
  switch (process_class) {
    case SYSTEM:
      return "system";

    case APPLICATION:
      return "application";

    default:
      return "UNKNOWN CLASS";
  }
}

const char* get_process_state_string(PROCESS_STATE process_state) {
  switch (process_state) {
    case READY:
    case SUSPENDED_READY:
      return "ready";

    case RUNNING:
      return "running";

    case BLOCKED:
    case SUSPENDED_BLOCKED:
      return "blocked";

    default:
      return "UNKNOWN STATE";
  }
}