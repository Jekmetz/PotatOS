#pragma once

#include "pcb_constants.h"


void enqueue(queue_t *que, pcb_t *data);

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
void priority_enqueue(queue_t *cue, pcb_t* data);

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
pcb_t *dequeue(queue_t *queue);

/**
 *  @brief Creates a queue
 * 
 *  Creates and allocates a queue and sets all variables correctly for initialization.
 * 
 *  @returns A pointer to a newly constructed queue.
 */
queue_t *construct_queue();

/**
 *  @brief Destructs a queue and its contents
 * 
 *  De-allocates a queue and all of the elements within it. This function
 *  exists to avoid memory leaks.
 * 
 *  @param queue A pointer to the queue you wish to deallocate.
 */
void destruct_queue(queue_t * queue);