#pragma once

#ifndef PCB_UTILS_H
#define PCB_UTILS_H

/**
* @file pcb_utils.h
*
* @brief Utility functions for all PCBs
*/

#include "pcb_queue.h"
#include "pcb_constants.h"

void showReadyQueue();

/**
* @brief simply allocates space for a pcb and returns that pointer
*
* @return pointer to allocated pcb
*/
pcb_t* allocate_pcb(char*);

/**
* @brief Setup a new PCB and enques that PCB
*
* @params PROCESS_CLASS The Process Class of the new PCB
* @params priority The priority of the new PCB
*/
pcb_t* setup_pcb(char*, PROCESS_CLASS, int priority);

/**
* @brief Frees the space for a pcb
*
* @return Success or failure
*/
int free_pcb(pcb_t*);

/**
 * @brief Finds a PCB in all queues.
 * 
 * Searches through all the system PCB queues to find a PCB with the specified
 * process name given by pname.
 * 
 * @param pname The name of the process you want to find the PCB of.
 * @return A pointer to the pcb with the specified name or 'NULL' for not found.
 */
pcb_t* find_pcb(char* pname);

/**
* @brief Inserts a PCB
*
* @params pcb_t The PCB to be inserted
*/
int insert_pcb(pcb_t*);

/**
 * @brief Removes a PCB by process name.
 * 
 * Searches through all system queues to find the PCB with the given name.
 * 
 * @param pname Name of the process you want to remove.
 * @return Success condition (boolean).
 */
pcb_t *remove_pcb(char* pname);

/**
* @brief Initializes queues
*
* @params None
*/
void init_queue();

/**
* @brief Getter function for the ready queue
*
* @return A pointer to the ready queue
*/
queue_t* get_ready_queue();

/**
* @brief Getter function for the blocked queue
*
* @return A pointer to the blocked queue
*/
queue_t* get_blocked_queue();

/**
* @brief Getter function for the suspended ready queue
*
* @return A pointer to the suspended ready queue
*/
queue_t* get_suspended_ready_queue();

/**
* @brief Getter function for the suspended blocked queue
*
* @return A pointer to the suspended blocked queue
*/
queue_t* get_suspended_blocked_queue();

/**
* @brief Prints the passed pcb's info in a stylized manner
*
* Example output
*
* Process Name: PROC1
* Process Class: system
* State: ready
* Priority: 1
* Suspended: true
*/
void print_pcb_info(const pcb_t* pcb);

/**
* @brief Returns a string corresponding to the process class enum
*
* @param process_class An enumeration variant of the PROCESS_CLASS enum
* @return A char pointer that is the enumeration name
*/
const char* get_process_class_string(PROCESS_CLASS process_class);

/**
* @brief Returns a string corresponding to the process state enum
*
* @param process_state An enumeration variant of the PROCESS_STATE enum
* @return A char pointer that is the enumeration name
*/
const char* get_process_state_string(PROCESS_STATE process_state);

/**
* @breif kills the ready queue after commhand death
*/
void kill_it___kill_it_all();

#endif
