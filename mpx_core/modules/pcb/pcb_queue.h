#pragma once

/**
 *  Contains all possible process classes
 * 
 *  TODO: there may be more
 */
enum PROCESS_CLASS
{
    SYSTEM,
    APPLICATION
};

/**
 *  Contains all possible process states
 */
enum PROCESS_STATE
{
    READY,
    RUNNING,
    BLOCKED
};

/**
 *  Struct that contains all information related to a pcb
 */
struct pcb
{
    char *process_name;
    unsigned int process_class;
    unsigned int priority;
    unsigned int state;
    char stack[1024];
};
typedef struct pcb pcb_t;

/**
 *  One element within the pcb queue
 * 
 *  This allows us to abbreviate code elsewhere... probably
 */
struct node
{
    pcb_t *data;
    
    void *next;
    void *prev;
};

/**
 *  Contains all the data needed to use/modify a queue
 */
struct queue
{
    int size;
    struct node *head;
    struct node *tail;  
};
typedef struct queue queue_t;

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

/**
 *  Attempts to find a process by name, and destroys it
 * 
 *  If this process cannot find the process, it will fail (return false/0)
 *  if the process is successfully immolated, it will succeed (return true/1)
 */
int remove_pcb(queue_t *queue, char *name);