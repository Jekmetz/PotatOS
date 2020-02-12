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
int setup_pcb(char*, PROCESS_CLASS, int priority);

// TODO
/**
*
*/
// Frees a pcb
int free_pcb(pcb_t*);

// TODO
/**
*
*/
pcb_t* find_pcb(char* pname);

// TODO
/**
*
*/
int insert_pcb(pcb_t*);

// TODO
/**
*
*/
int remove_pcb(char*);

// TODO
/**
*
*/
void init_queue();

