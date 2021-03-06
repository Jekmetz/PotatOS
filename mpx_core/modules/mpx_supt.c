/*************************************************************
 *	This C file contains the MPX support functions
 *	which will be used through out the semester, many set
 *	flags or methods that will allow us to modify
 *	The behavior of MPX as it progresses throughout
 * 	the semester.
 **************************************************************/
#include "mpx_supt.h"
#include "./commands/time.h"
#include "./pcb/pcb_queue.h"

//TMP 
#include "./pcb/pcb_utils.h"

#include "./memory_management/memory_wrangler.h"

// global variable containing parameter used when making
// system calls via sys_req
param params;

// global variable containing context of the currently running process
context_t* gcontext;
pcb_t* cop = NULL;

// global for the current module
int current_module = -1;
static int io_module_active = 0;
static int mem_module_active = 0;

// If a student created heap manager is implemented this
// is a pointer to the student's "malloc" operation.
void* (*student_malloc)(u32int);

void* (*student_malloc_named)(u32int,char*);

//Function to save the context from the source to the dest
void save_context(context_t* dest, context_t* source);

// if a student created heap manager is implemented this
// is a pointer to the student's "free" operation.
int (*student_free)(void*);

int (*student_read)(char* buffer, int* count);

/* *********************************************
 *	This function is use to issue system requests
 *	for service.
 *
 *	Parameters:  op_code:  Requested Operation, one of
 *					READ, WRITE, IDLE, EXIT
 *			  device_id:  For READ & WRITE this is the
 *					  device to which the request is
 *					  sent.  One of DEFAULT_DEVICE or
 *					   COM_PORT
 *			   buffer_ptr:  pointer to a character buffer
 *					to be used with READ & WRITE request
 *			   count_ptr:  pointer to an integer variable
 *					 containing the number of characters
 *					 to be read or written
 *
 *************************************************/
int sys_req(int op_code, int device_id, char* buffer_ptr, int* count_ptr)
{
  int return_code = 0;

  if (op_code == IDLE || op_code == EXIT) {
    // store the process's operation request
    // triger interrupt 60h to invoke
    params.op_code = op_code;
    asm volatile("int $60");
  }  // idle or exit

  else if (op_code == READ || op_code == WRITE) {
    // validate buffer pointer and count pointer
    if (buffer_ptr == NULL)
      return_code = INVALID_BUFFER;
    else if (count_ptr == NULL || *count_ptr <= 0)
      return_code = INVALID_COUNT;

    // if parameters are valid store in the params structure
    if (return_code == 0) {
      params.op_code = op_code;
      params.device_id = device_id;
      params.buffer_ptr = buffer_ptr;
      params.count_ptr = count_ptr;

      if (!io_module_active) {
        // if default device
        if (op_code == READ)
          return_code = student_read(buffer_ptr, count_ptr);

        else  // must be WRITE
          return_code = serial_print(buffer_ptr);

      } else {  // I/O module is implemented
        asm volatile("int $60");
      }  // NOT IO_MODULE
    }
  } else
    return_code = INVALID_OPERATION;

  return return_code;
}  // end of sys_req

u32int* sys_call(context_t* registers)
{
  queue_t* ready_queue = get_ready_queue();
  pcb_t* tmp = NULL;

  if(cop == NULL) //if sys_call has not been called...
  {
    //save global context
    gcontext = registers;
  } else //if sys_call has been called
  {
    if(params.op_code == EXIT)
    {
      //kill it with fire
      free_pcb(cop);
    } else
    {
      cop->stacktop = (unsigned char*)registers;
      cop->state = READY;

      struct fakelong tim = rdtsc();
      // unsigned int* shft = (unsigned int*)(((unsigned char*)&tim) + 16);
      u32int mid = (tim.upper << 16) + (tim.lower >> 16);
      cop->last_time_run = mid;
      tmp = cop;
    }
  }

  if(ready_queue->size > 0) //if we have a ready process...
  {
    cop = dequeue(ready_queue);
    cop->state = RUNNING;
    if(tmp != NULL) insert_pcb(tmp);
    return (u32int*)cop->stacktop;
  }

  return (u32int*)gcontext;
}

void process_loader(char* proc_name, int priority, int class, void (*func)(void), u32int state)
{
  //dont load a duplicate process
  if(find_pcb(proc_name) != NULL) return;

  pcb_t* new_pcb = setup_pcb(proc_name, class, priority);
  context_t* cp = (context_t*)(new_pcb->stacktop);
  memset(cp, 0, sizeof(context_t));
  cp->fs=0x10;
  cp->gs=0x10;
  cp->ds=0x10;
  cp->es=0x10;
  cp->cs=0x08;
  cp->ebp=(u32int)(new_pcb->stack);
  cp->esp=(u32int)(new_pcb->stacktop);
  cp->eip=(u32int)func;
  cp->eflags=0x202;
  new_pcb->state = state;
  insert_pcb(new_pcb);
}

/*
  Procedure..: mpx_init
  Description..: Initialize MPX support software, based
                        on the current module.  The operation of
                        MPX will changed based on the module selected.
                        THIS must be called as the first executable
                        statement inside your command handler.

  Params..: int cur_mod (symbolic constants MODULE_R1, MODULE_R2,
  etc.  These constants can be found inside mpx_supt.h
*/
void mpx_init(int cur_mod) {
  current_module = cur_mod;
  if (cur_mod == MEM_MODULE || cur_mod == MODULE_R5)
  {
    mem_module_active = TRUE;
    student_malloc_named = &internal_malloc_named;
  }

  if (cur_mod == IO_MODULE)
    io_module_active = TRUE;
}

/*
  Procedure..: sys_set_read
  Description..: Sets the default function for reading input
                        from the keyboard, to the function developed
                        by the students
  Params..: Function pointer
*/
void sys_set_read(int (*func)(char* b, int* cnt)) {
  student_read = func;
}

/*
  Procedure..: sys_set_malloc
  Description..: Sets the memory allocation function for sys_alloc_mem
  Params..: Function pointer
*/
void sys_set_malloc(void* (*func)(u32int)) {
  student_malloc = func;
}

/*
  Procedure..: sys_set_free
  Description..: Sets the memory free function for sys_free_mem
  Params..: s1-destination, s2-source
*/
void sys_set_free(int (*func)(void*)) {
  student_free = func;
}

/*
  Procedure..: sys_alloc_mem
  Description..: Allocates a block of memory (similar to malloc)
  Params..: Number of bytes to allocate
*/
void* sys_alloc_mem(u32int size) {
  if (!mem_module_active)
    return (void*)kmalloc(size);
  else
    return (void*)(*student_malloc)(size);
}

void* sys_alloc_mem_named(u32int size, char* pname) {
  if(!mem_module_active)
    return (void*)kmalloc(size);
  else 
    return (void*)(*student_malloc_named)(size,pname);
}

/*
  Procedure..: sys_free_mem
  Description..: Frees memory
  Params..: Pointer to block of memory to free
*/
int sys_free_mem(void* ptr) {
  if (mem_module_active)
    return (*student_free)(ptr);
  // otherwise we don't free anything
  return -1;
}

/*
  Procedure..: idle
  Description..: The idle process, used in dispatching
                        it will only be dispatched if NO other
                        processes are available to execute.
  Params..: None
*/
void idle() {
  char msg[30];
  int count = 0;

  memset(msg, '\0', sizeof(msg));
  strcpy(msg, "IDLE PROCESS EXECUTING.\n");
  count = strlen(msg);

  while (1) {
    sys_req(WRITE, DEFAULT_DEVICE, msg, &count);
    sys_req(IDLE, DEFAULT_DEVICE, NULL, NULL);
  }
}

const pcb_t* get_running_process() {
  return cop;
}
