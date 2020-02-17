#include "pcb_queue.h"
#include "pcb_constants.h"

// TODO
/**
*
*/
// Just allocates ips a pcb and returns the pointer to that pcb
pcb_t* allocate_pcb();

// TODO
/**
*
*/
// Setups a pcb and enques the pcb
// pointer to pcb, name of the process, ...
pcb_t *setup_pcb(char*, PROCESS_CLASS, int priority);

// TODO
/**
*
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

