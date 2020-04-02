/**
* @file memory_wrangler.h
*
* @brief The header file for the memory control and memory functions
*/

#include "../pcb/pcb_constants.h"

/**
* @brief The overall Heap Size
*/
#define HEAP_SIZE 54301

/**
* @brief MCB_PADDING Macro
*/
#define MCB_PADDING (sizeof(cmcb) + sizeof(lmcb))

/**
* @brief Enum to specify weather memory is free or alive
*/
typedef enum { FREE, ALIVE } MEMTYPE;

/**
* @brief CMCB Struct
*/
typedef struct {
  char* karen;
  unsigned int size;
  MEMTYPE type;
} cmcb;

/**
* @brief LMCB Struct
*/
typedef struct {
  cmcb* top;
  MEMTYPE type;
} lmcb;

/*
* @brief Memory initializing function
*
* @paramas None
*/
void* mem_init();

/**
* @brief Internal memory allocation function
*
* @params size The size of the memory that needs to be allocated
*/
void* internal_malloc(u32int size);

void* internal_malloc_named(u32int size, char* karen);

/**
* @brief Internal memory free function
*
* @params data The data that needs to be freed
*/
int internal_free(void* data);

/**
* @brief Get the remaining free memory
*
* @params None 
*/
u32int get_remaining_free();

/**
* @brief prints out cmcb info
*
* @params cmcb The requested CMCB
*/
void print_cmcb(cmcb*);

/**
* @brief prints out lmcb info
*
* @params lmcb The requested LMCB
*/
void print_lmcb(lmcb*);

/**
* @brief prints out cmcb info and lmcb info with a message
*
* @params c The CMCB requested
* @params l The LMCB requested
* @params msg The message to be displayed
*/
void print_both(cmcb* c, lmcb* l, char* msg);

/**
* @breif show the entire state of memory visualized
*
* @params None 
*/
void show_mem_state();

/**
* @brief Function to show the free memory state
*
* @params None
*/
void show_free_mem_state();

/**
* @brief Function to show the allocated memory state
*
* @params None
*/
void show_alloc_mem_state();