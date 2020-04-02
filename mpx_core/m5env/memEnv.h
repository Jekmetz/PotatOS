/**
* @file memEnv.h
*
* @brief The header file for the memory env files
*/

/**
@ brief Macro of the heap size
*/
#define HEAP_SIZE 1000

/**
* @brief Macro of the MCB_Padding
*/
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

/**
* @brief CMCB struct
*/
typedef struct {
	pcb_t* karen;
	unsigned int size;
	MEMTYPE type;
} cmcb;

/**
* @brief LMCB struct
*/
typedef struct {
	cmcb* top;
	MEMTYPE type;
} lmcb;

/**
* @brief Memory init function
* 
* @params None
*/
void* mem_init();

/**
* @brief Internal memory allocation function
*
* @parms size The size of the memory to be allocated
*/
void* internal_malloc(u32int size);

/**
* @brief Internal free memory function
* 
* @params data The data to be freed
*/
int internal_free(void* data);

/*
* @brief Function to show the interanl free memory
*
* @params none
*/
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