#include "../pcb/pcb_constants.h"

#define HEAP_SIZE 54301
#define MCB_PADDING (sizeof(cmcb) + sizeof(lmcb))

typedef enum { FREE, ALIVE } MEMTYPE;

typedef struct {
  char* karen;
  unsigned int size;
  MEMTYPE type;
} cmcb;

typedef struct {
  cmcb* top;
  MEMTYPE type;
} lmcb;

void* mem_init();

void* internal_malloc(u32int size);

void* internal_malloc_named(u32int size, char* karen);

int internal_free(void* data);

u32int get_remaining_free();

/**
* @brief prints out cmcb info
*/
void print_cmcb(cmcb*);

/**
* @brief prints out lmcb info
*/
void print_lmcb(lmcb*);

/**
* @brief prints out cmcb info and lmcb info with a message
*/
void print_both(cmcb* c, lmcb* l, char* msg);

/**
* @breif show the entire state of memory visualized
*/
void show_mem_state();

void show_free_mem_state();

void show_alloc_mem_state();