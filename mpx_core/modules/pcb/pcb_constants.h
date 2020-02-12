#pragma once

 /**
 *  Contains all possible process classes
 * 
 *  TODO: there may be more
 */
typedef enum
{
    SYSTEM,
    APPLICATION
} PROCESS_CLASS;

/**
 *  Contains all possible process states
 */
typedef enum
{
    READY,
    RUNNING,
    BLOCKED
} PROCESS_STATE;

/**
 *  Struct that contains all information related to a pcb
 */
typedef struct pcb
{
    char *process_name;
    unsigned int process_class;
    unsigned int priority;
    unsigned int state;
    char stack[1024];
} pcb_t;

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
typedef struct queue
{
    int size;
    struct node *head;
    struct node *tail;  
} queue_t;