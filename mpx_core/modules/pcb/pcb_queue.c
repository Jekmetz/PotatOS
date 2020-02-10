#include "pcb_queue.h"
#include "../mpx_supt.h"

void enqueue(queue_t *que, pcb_t *data)
{
    struct node *nnode = sys_alloc_mem(sizeof(struct node));
    nnode->data = data;
    nnode->prev = que->size > 0 ? que->tail : NULL;
    nnode->next = NULL;
    que->tail = nnode;
    if (que->size == 0)
    {
        que->head = nnode;
    }
    que->size++;
}

pcb_t *dequeue(queue_t *queue)
{
    pcb_t *ret = queue->head->data;
    struct node *ded = queue->head;
    queue->head = queue->head->next;
    queue->head->prev = NULL;
    sys_free_mem(ded);
    queue->size--;
    return ret;
}

pcb_t *peek(queue_t *queue)
{
    return queue->head->data;
}

queue_t *construct_queue()
{
    queue_t *que = sys_alloc_mem(sizeof(queue_t));
    que->head = NULL;
    que->tail = NULL;
    que->size = 0;
    return que;
}

void destruct_queue(queue_t *queue)
{
    while (queue->size > 0)
    {
        dequeue(queue);
    }
    sys_free_mem(queue);
}