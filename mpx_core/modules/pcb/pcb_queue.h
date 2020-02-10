#pragma once

enum PROCESS_CLASS
{
    SYSTEM,
    APPLICATION
};

enum PROCESS_STATE
{
    READY,
    RUNNING,
    BLOCKED
};

enum SUCCESS_STATES
{
    FAILED,
    SUCCESS,
    NOT_FOUND
};

struct pcb
{
    char *process_name;
    unsigned int process_class;
    unsigned int priority;
    unsigned int state;
    char stack[1024];
};
typedef struct pcb pcb_t;

struct node
{
    pcb_t *data;
    
    void *next;
    void *prev;
};

struct queue
{
    int size;
    struct node *head;
    struct node *tail;  
};
typedef struct queue queue_t;

void enqueue(queue_t *que, pcb_t *data);

pcb_t *dequeue(queue_t *queue);

pcb_t *peek(queue_t *queue);

queue_t *construct_queue();

void destruct_queue(queue_t *);

int kill_process(char *name);