#pragma once

#include "pcb_constants.h"

/**
 *  Appends an element onto the tail of the given queue
 * 
 *  This also allocates memory for a node
 */
void enqueue(queue_t *que, pcb_t *data);

/** 
 *  Takes the PCB off of the head of the queue and moves head
 * 
 *  Takes care of freeing the node
 *  returns the head PCB
 */
pcb_t *dequeue(queue_t *queue);

/**
 *  Creates a queue
 * 
 *  Creates a queue and sets all variables correctly for initialization
 */
queue_t *construct_queue();

/**
 *  Destructs a queue
 * 
 *  De-allocates a queue and all of the elements within it.
 */
void destruct_queue(queue_t *);