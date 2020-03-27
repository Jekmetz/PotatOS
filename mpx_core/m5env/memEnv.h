#define HEAP_SIZE 300
#define MCB_PADDING (sizeof(cmcb) + sizeof(lmcb))

//Compat
typedef unsigned int u32int;

typedef enum { FREE, ALIVE } MEMTYPE;

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

void* internal_malloc(u32int size);

int internal_free(void* data);

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