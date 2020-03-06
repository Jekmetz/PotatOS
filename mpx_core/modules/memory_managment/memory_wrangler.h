#include "../pcb/pcb_constants.h"

#define HEAP_SIZE 54321
#define MCB_PADDING (sizeof(cmcb) + sizeof(lmcb))


typedef enum
{
    FREE,
    ALIVE
} MEMTYPE;

typedef struct 
{
    pcb_t *karen;
    unsigned int size;
    MEMTYPE type;
} cmcb;

typedef struct
{
    cmcb *top;
    MEMTYPE type;
} lmcb;

int init();

void *internal_malloc(unsigned int size, pcb_t *karen);

int internal_free(void * data);
