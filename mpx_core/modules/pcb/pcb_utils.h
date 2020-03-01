/**
* @file pcb_utils.h
*
* @brief Utility functions for all PCBs
*/

#pragma once

#include "pcb_queue.h"
#include "pcb_constants.h"

/**
* @brief simply allocates space for a pcb and returns that pointer
*
* @return pointer to allocated pcb
*/
pcb_t* allocate_pcb();

// TODO
/**
*
*/
// Setups a pcb and enques the pcb
// pointer to pcb, name of the process, ...
pcb_t* setup_pcb(char*, PROCESS_CLASS, int priority);

// TODO
/**
* @brief frees the space for a pcb
*
* @return Success or failure
*/
// Frees a pcb
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

// TODO
/**
*
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

// TODO
/**
*
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
* @brief Getter function for the running process
*
* @return A pointer to the running process
*/
pcb_t** get_running_process();

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
