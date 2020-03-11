#include "pcb_queue.h"
#include "../mpx_supt.h"

#include <string.h>
#include <core/stdio.h>
#include "../commands/time.h"

// definitions of success and failure for true/false returns
#define SUCCESS 1
#define FAILURE 0

#define TIME_FLUX ((u32int)1000)

// #define DEBUG(msg, dat...) printf("\033[32;1mQUEUE: "msg"\n\033[0m", dat)

// TODO: REMOVE WHEN M5
#define MAX_PROCS 15
node_t nelly[MAX_PROCS] = {(node_t){NULL, NULL, NULL, 0}};

node_t* local_alloc_mem() {
  int idx = 0;
  while (nelly[idx].flag != 0)
    idx++;

  nelly[idx].flag = 1;
  return &(nelly[idx]);
}

void local_free_mem(node_t* thing) {
  thing->flag = 0;
  return;
}
// TODO: DONE

unsigned int cc_mid32() {
  struct fakelong tim = rdtsc();
  u32int mid = (tim.upper << 16) + (tim.lower >> 16);
  return mid;
}

/**
 * @brief Appends an element to the end of the queue
 *
 * This function searches for the end of the queue and, adds the specified pcb
 * to the end of the list.
 *
 * @param que A pointer to a queue that the PCB will be inserted into.
 * @param data A pointer to the PCB to insert into the queue.
 */
void enqueue(queue_t* cue, pcb_t* data) {
  // struct node* nnode = sys_alloc_mem(sizeof(struct node));
  // TODO: REMOVE WHEN M5
  node_t* nnode = local_alloc_mem();

  // DEBUG("ENQUEUE: adding %s", data->process_name);

  // setting node data
  nnode->data = data;
  nnode->prev = NULL;
  nnode->next = NULL;

  // edge case for idle process (that should never run)
  if (cue->head != NULL && strcmp(cue->head->data->process_name, "idle") == 0) {
    cue->head->prev = nnode;
    nnode->next = cue->head;
    cue->head = nnode;

    // DEBUG("ENQUEUE: Avoiding idle process in %s", data->process_name);
  }

  if (cue->head == NULL) {
    cue->head = nnode;
    // DEBUG("ENQUEUE: Head/tail is now %s", data->process_name);
  } else {
    struct node* curr = cue->head;
    while (curr->next != NULL &&
           strcmp(((node_t*)(curr->next))->data->process_name, "idle") != 0) {
      curr = curr->next;
    }
    curr->next = nnode;
    nnode->prev = curr;
    // DEBUG("ENQUEUE: Tail is now %s : pv=%s", data->process_name,
    // ((node_t*)(nnode->prev))->data->process_name);
  }
}

/**
 *  @brief Appends an element onto the tail of the given queue
 *
 *  This function inserts the given data (a PCB) into the queue according to
 *  priority.
 *
 *  @param que A poiter to the queue to insert the data into.
 *  @param data a pointer to the PCB that is to be inserted.
 *
 *  @note The data must point to a pcb with a valid priority.
 */
void priority_enqueue(queue_t* cue, pcb_t* data) {
  // allocating node space
  // struct node* nnode = sys_alloc_mem(sizeof(struct node));

  // TODO: REMOVE WHEN M5
  node_t* nnode = local_alloc_mem();

  u32int now = cc_mid32();
  // setting node data
  nnode->data = data;
  nnode->prev = NULL;
  nnode->next = NULL;
  nnode->data->last_time_run = now;

  if (cue->size == 0) {
    cue->head = nnode;
  } else {

    // getting the time addative for the head node
    u32int head_time_piority =
        TIME_FLUX / (now - cue->head->data->last_time_run + 1);
    // getting the time addative for the new node
    u32int node_time_piority =
        TIME_FLUX / (now - nnode->data->last_time_run + 1);

    // comparing priorities of head and new node as edge case
    if (cue->head->data->priority + head_time_piority >
        nnode->data->priority + node_time_piority) {
      cue->head->prev = nnode;
      nnode->next = cue->head;
      cue->head = nnode;
    } else {  // if not that edge case
      // iterating through all the nodes
      struct node* curr = cue->head;
      u32int curr_time_priority = head_time_piority;
      while (curr->next != NULL &&
             curr->data->priority + curr_time_priority <=
                 nnode->data->priority + node_time_piority) {
        curr = curr->next;
        curr_time_priority = TIME_FLUX / (now - curr->data->last_time_run + 1);
      }

      // checking end edge case
      // and inserting new node into queue
      if (curr->data->priority + curr_time_priority <=
          nnode->data->priority + node_time_piority) {
        curr->next = nnode;
        nnode->prev = curr;
      } else {
        ((struct node*)curr->prev)->next = nnode;
        nnode->next = curr;
        nnode->prev = curr->prev;
        curr->prev = nnode;
      }
    }
  }

  // incrementing size
  cue->size++;
}

/**
 *  @brief Takes the PCB off of the head of the queue and moves head
 *
 *  Takes care of freeing the node
 *  returns the head PCB
 *
 *  @param queue A pointer to a queue that you want to dequeue the first element
 * from.
 *
 *  @returns A pointer to the dequed PCB
 */
pcb_t* dequeue(queue_t* queue) {
  // getting head
  pcb_t* ret = queue->head->data;
  // DEBUG("DEQUEUE: dequeueing process %s", ret->process_name);
  // saving head in a local pointer
  struct node* ded = queue->head;
  // shifting head
  queue->head = queue->head->next;
  // removing reference to old head
  queue->head->prev = NULL;

  // freeing old head
  sys_free_mem(ded);
  local_free_mem(ded);
  // decrementing queue
  queue->size--;

  // returning dequeued data
  return ret;
}

/**
 *  @brief Creates a queue
 *
 *  Creates and allocates a queue and sets all variables correctly for
 * initialization.
 *
 *  @returns A pointer to a newly constructed queue.
 */
queue_t* construct_queue() {
  // allocating queue
  queue_t* que = sys_alloc_mem(sizeof(queue_t));
  // setting data
  que->head = NULL;
  que->size = 0;
  // returning pointer
  return que;
}

/**
 *  @brief Destructs a queue and its contents
 *
 *  De-allocates a queue and all of the elements within it. This function
 *  exists to avoid memory leaks.
 *
 *  @param queue A pointer to the queue you wish to deallocate.
 */
void destruct_queue(queue_t* queue) {
  // checking if the que has any contents
  struct node* curr = queue->head;
  while (queue->size > 0) {
    // destroying data
    curr = curr->next;
    sys_free_mem(curr->prev);
    queue->size--;
  }
  // freeing queue
  sys_free_mem(queue);
}