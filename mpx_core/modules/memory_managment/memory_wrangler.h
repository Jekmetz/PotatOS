#include "../pcb/pcb_constants.h"

#define HEAP_SIZE 54301
#define MCB_PADDING (sizeof(cmcb) + sizeof(lmcb))

typedef enum { FREE, ALIVE } MEMTYPE;

typedef struct {
  pcb_t* karen;
  unsigned int size;
  MEMTYPE type;
} cmcb;

typedef struct {
  cmcb* top;
  MEMTYPE type;
} lmcb;

void* mem_init();

u32int internal_malloc(u32int size);

int internal_free(void* data);
