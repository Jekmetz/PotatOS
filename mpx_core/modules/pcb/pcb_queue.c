#include "pcb_queue.h"
#include "../mpx_supt.h"

#include <string.h>

// definitions of success and failure for true/false returns
#define SUCCESS 1
#define FAILURE 0

void enqueue(queue_t *que, pcb_t *data)
{
    // allocating node space
    struct node *nnode = sys_alloc_mem(sizeof(struct node));

    // setting node data
    nnode->data = data;
    nnode->prev = que->size > 0 ? que->tail : NULL;
    nnode->next = NULL;

    // appending new node to tail
    que->tail->next = nnode;
    que->tail = nnode;

    // checking for edge case of no data in queue
    if (que->size == 0)
    {
        que->head = nnode;
    }
    // incrementing size
    que->size++;
}

pcb_t *dequeue(queue_t *queue)
{
    // getting head
    pcb_t *ret = queue->head->data;
    // saving head in a local pointer
    struct node *ded = queue->head;
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

queue_t *construct_queue()
{
    // allocating queue
    queue_t *que = sys_alloc_mem(sizeof(queue_t));
    // setting data
    que->head = NULL;
    que->tail = NULL;
    que->size = 0;
    // returning pointer
    return que;
}

void destruct_queue(queue_t *queue)
{
    // checking if the que has any contents
    struct node *curr = queue->head;
    while (queue->size > 0)
    {
        // destroying data
        curr = curr->next;
        sys_free_mem(curr->prev);
        queue->size--;
    }
    // freeing queue
    sys_free_mem(queue);
}

/**
 *  FIXME: This is not merciless enough
 */
int remove_pcb(queue_t *que, char *name)
{
    // setting the moving pointer to start at head
    struct node *curr = que->head;
    while (curr != NULL)
    {
        // checking if the process is the correct one
        if (strcmp(curr->data->process_name, name) == 0)
        {
            // destroying the process
            ((struct node *)curr->prev)->next = curr->next;
            ((struct node *)curr->next)->prev = curr->prev;
            sys_free_mem(curr);
            que->size--;
            return SUCCESS;
        }
        curr = curr->next;
    }
    return FAILURE;
}