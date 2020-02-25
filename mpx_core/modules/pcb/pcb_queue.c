#include "pcb_queue.h"
#include "../mpx_supt.h"

#include <string.h>
#include <core/stdio.h>

// definitions of success and failure for true/false returns
#define SUCCESS 1
#define FAILURE 0

/**
 * @brief Appends an element to the end of the queue
 * 
 * This function searches for the end of the queue and, adds the specified pcb to
 * the end of the list.
 * 
 * @param que A pointer to a queue that the PCB will be inserted into.
 * @param data A pointer to the PCB to insert into the queue.
 */
void enqueue(queue_t *cue, pcb_t *data)
{
  struct node* nnode = sys_alloc_mem(sizeof(struct node));

  // setting node data
  nnode->data = data;
  nnode->prev = NULL;
  nnode->next = NULL;

  if (cue->head == NULL)
  {
    cue->head = nnode;
  }
  else
  {
    struct node *curr = cue->head;
    while (curr->next != NULL)
    {
      curr = curr->next;
    }

    curr->next = nnode;
    nnode->prev = curr;
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
  struct node* nnode = sys_alloc_mem(sizeof(struct node));

  // setting node data
  nnode->data = data;
  nnode->prev = NULL;
  nnode->next = NULL;

  if (cue->size == 0) {
    cue->head = nnode;
  } else {
    if (cue->head->data->priority > nnode->data->priority) {
        cue->head->prev = nnode;
        nnode->next = cue->head;
        cue->head = nnode;
    } else {
      struct node* curr = cue->head;
      while (curr->next != NULL &&
             curr->data->priority <= nnode->data->priority) {
        curr = curr->next;
      }

      if (curr->data->priority <= nnode->data->priority) {
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
 *  @param queue A pointer to a queue that you want to dequeue the first element from.
 * 
 *  @returns A pointer to the dequed PCB
 */
pcb_t* dequeue(queue_t* queue) {
  // getting head
  pcb_t* ret = queue->head->data;
  // saving head in a local pointer
  struct node* ded = queue->head;
  // shifting head
  queue->head = queue->head->next;
  // removing reference to old head
  queue->head->prev = NULL;

  // freeing old head
  sys_free_mem(ded);
  // decrementing queue
  queue->size--;

  // returning dequeued data
  return ret;
}

/**
 *  @brief Creates a queue
 * 
 *  Creates and allocates a queue and sets all variables correctly for initialization.
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