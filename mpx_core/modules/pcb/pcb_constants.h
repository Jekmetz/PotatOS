#pragma once

#ifndef PCB_CONSTANTS_H
#define PCB_CONSTANTS_H

/**
* @file pcb_constants.h
*
* @brief Contains all shared resources amongst all PCBs
*/

#define DEFAULT_PRIORITY 314159265

 /**
 *  @brief Contains all possible process classes
 * 
 */
typedef enum
{
    SYSTEM,
    APPLICATION
} PROCESS_CLASS;

/**
 *  @brief Contains all possible process states
 */
typedef enum
{
    RUNNING,
    READY,
    BLOCKED,
    SUSPENDED_READY,
    SUSPENDED_BLOCKED
} PROCESS_STATE;

/**
 *  @brief Struct that contains all information related to a pcb
 */
typedef struct pcb
{
    char *process_name;
    u32int process_class;
    u32int priority;
    u32int last_time_run;
    u32int state;
    char stack[2048];
    unsigned char *stacktop;
} pcb_t;

/**
 *  @brief One element within the pcb queue
 * 
 *  This allows us to abbreviate code elsewhere... probably
 */
typedef struct node
{
    pcb_t *data;
    
    void *next;
    void *prev;
} node_t;

/**
 *  @brief Contains all the data needed to use/modify a queue
 */
typedef struct queue
{
    int size;
    struct node *head;
    struct node *tail;  
} queue_t;

#endif