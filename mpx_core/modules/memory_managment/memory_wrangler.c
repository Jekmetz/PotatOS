#include "memory_wrangler.h"
#include "../mpx_supt.h"
#include <system.h>
#include <mem/heap.h>

void* fma;
cmcb* ffree;
cmcb* falive;

unsigned int remaining_free;

#define SUCCESS 1
#define FAILURE 0

#define DEBUG 1

#if DEBUG
#include <core/stdio.h>
#define PRINT(str, dat...) \
  printf("\033[35mMEM_WRANGLER(%d):\033[0m " str "\033[0m\n", __LINE__, dat)
#endif

#define NEXT_BLOCK(block) \
  (cmcb*)((unsigned char*)block + block->size + MCB_PADDING)
#define GET_LMCB(block) \
  (lmcb*)((unsigned char*)block + block->size + sizeof(cmcb))
#define ASCMCB(loc) \
  ((cmcb*)loc)
#define ASLMCB(loc) \
  ((lmcb*)loc)

typedef unsigned char* location;

void* mem_init() {
  // Creating heap
  fma = (void*)kmalloc(HEAP_SIZE + MCB_PADDING);

  // Setting free block
  ffree = fma;
  falive = NULL;
  ffree->karen = NULL;
  ffree->size = HEAP_SIZE;
  ffree->type = FREE;

  // ending block
  lmcb* end = (lmcb*)((location)fma + sizeof(cmcb) + HEAP_SIZE);
  end->top = ffree;
  end->type = FREE;

  remaining_free = HEAP_SIZE;

#if DEBUG
  PRINT("Created heap with \033[32;1m%d bytes", HEAP_SIZE + MCB_PADDING);
#endif
  return fma;
}

void* internal_malloc(u32int size) {
  pcb_t* karen = (pcb_t*)get_running_process();

#if DEBUG
  if (karen != NULL) {
    PRINT("Allocating memory for %s - %d", karen->process_name, size);
  }
  else {
    PRINT("Allocating memory for NULL - %d bytes", size);
  }
#endif
    // checking that there is enough memory in the heap
  if (remaining_free < size + MCB_PADDING) {
    return NULL;
  }

  // finding first available free block
  /*Algorithm explained:
  * 1. set the current block to the first free block
  * 2. While we have not run out of space and while the 
       current blocks size is less than the size we
  *    want to allocate OR the current block is ALIVE,
  * 3. Move current block to the next one
  *   3a. If we are out of space after we move, kill it with fire.
  */
  cmcb* curr_blk = ffree;
  while (
        ((location)curr_blk < (location)fma + HEAP_SIZE + MCB_PADDING)
        && (curr_blk->size < size || curr_blk->type == ALIVE)
        ) {
    curr_blk = NEXT_BLOCK(curr_blk);
    if ((location)curr_blk > (location)fma + HEAP_SIZE + sizeof(cmcb)) {
      return NULL;
      //curr_blk = ffree;
    }
  }

  // creating block
  unsigned int prev_size = curr_blk->size;
  curr_blk->karen = karen;
  curr_blk->type = ALIVE;
  curr_blk->size = size;

  // if this is the first allocation to the stack or we are less than
  // the first alive's location, set it.
  if( falive == NULL || (location)curr_blk < (location)falive)
  {
    falive = curr_blk;
  }

  //If there is not enough space to fit another block padding and an int...
  //Just give that block the space.
  if( (prev_size - size) < MCB_PADDING + sizeof(int))
  {
    lmcb* e_blk = (lmcb*)((location)curr_blk + prev_size + sizeof(cmcb));
    curr_blk->size = prev_size;
    e_blk->top = curr_blk;
    e_blk->type = ALIVE;
    //we just allocated a prev_size but we kept the control blocks!
    remaining_free -= prev_size;

    //If we have done this, we must search through to find the first free
    //provided of course one exists! (remaining_free > 0)
    if(remaining_free != 0)
    {
      //if we are before or at the first free with this new allocation...
      // less than should never proc but it is a safety measure
      if(curr_blk <= ffree)
      {
        cmcb* search = NEXT_BLOCK(curr_blk);
        while(search->type == ALIVE)
        {
          //skip it.
          search = NEXT_BLOCK(search);
        }

        //assumedly we are at the first free block now
        ffree = search;
      }
  } else
  {
    ffree = NULL;
  }

    return (void*)((location)curr_blk + sizeof(cmcb));
  }

  //If there is enough space to fit another Free block...

  lmcb* e_blk = (lmcb*)((location)curr_blk + size + sizeof(cmcb));
  e_blk->top = curr_blk;
  e_blk->type = ALIVE;

  // making the new free block with the leftovers
  cmcb* nfree = (cmcb*)((location)e_blk + sizeof(lmcb));
  nfree->karen = NULL;
  nfree->size = prev_size - size - MCB_PADDING;
  nfree->type = FREE;
  if (curr_blk == ffree) {
    ffree = nfree;
  }

  //Making the new free block lmcb
  lmcb* fend = (lmcb*)((location)nfree + nfree->size + sizeof(cmcb));
  fend->top = nfree;
  fend->type = FREE;

  //we just allocated a size plus a couple of control blocks!
  remaining_free -= size + MCB_PADDING;

  #if DEBUG
    print_both(curr_blk, e_blk, "JUST AFTER ALLOCATION");
  #endif

  show_mem_state();

  return (void*)((location)curr_blk + sizeof(cmcb));
}

int internal_free(void* data) {
  cmcb* block = data - sizeof(cmcb);
  lmcb* end = data + block->size;

  #if DEBUG
  if (block->karen != NULL){
    PRINT("Freeing memory @%x from process %s - %d bytes", data, block->karen->process_name, block->size);
  } else {
    PRINT("Freeing memory @%x from process NULL - %d bytes", data, block->size);
  }
  print_both(block,end,"JUST BEFORE FREEING");
  #endif

  // Setting variables of current block
  block->type = FREE;
  end->type = FREE;
  block->karen = NULL;
  remaining_free += block->size;

  // checking previous block
  //set the previous block if we have access to the memory and its type is free.
  lmcb* prev_lmcb = (lmcb*)((location)block - sizeof(lmcb));
  if ((location)prev_lmcb >= (location)fma && prev_lmcb->type == FREE) {
    prev_lmcb->top->size += block->size + MCB_PADDING;
    end->top = prev_lmcb->top;
    block = end->top;
    remaining_free += MCB_PADDING;
  }

  // checking next block
  //assimilate the next block if we have access to the memory and its type is free.
  cmcb* next_cmcb = (cmcb*)((location)block + MCB_PADDING + block->size);
  if ((location)next_cmcb < ((location)fma + HEAP_SIZE + MCB_PADDING) &&
      next_cmcb->type == FREE) {
    lmcb* next_lmcb = GET_LMCB(next_cmcb);
    next_lmcb->top = block;
    block->size += next_cmcb->size + MCB_PADDING;
    remaining_free += MCB_PADDING;
  }

  if (block < ffree) {
    ffree = block;
    falive = NEXT_BLOCK(block);
  }

  PRINT("remaining_free: %d",get_remaining_free());

  return SUCCESS;
}

u32int get_remaining_free()
{
  return remaining_free;
}

void print_cmcb(cmcb* p)
{
  printf(
    "CMCB INFO: (%d BYTES)\n"
    "karen: %s\n"
    "cmcbloc: %x\n"
    "datloc: %x\n"
    "size: %d\n"
    "type: %s\n\n",
    sizeof(cmcb),
    (p->karen != NULL) ? p->karen->process_name : "NULL",
    p,
    (location)p + sizeof(cmcb),
    p->size,
    (p->type == 0) ? "FREE" : "ALIVE"
    );
}

void print_lmcb(lmcb* p)
{
    printf(
    "LMCB INFO: (%d BYTES)\n"
    "lmcbloc: %x\n"
    "top: %x\n"
    "type: %s\n\n",
    sizeof(lmcb),
    p,
    p->top,
    (p->type == 0) ? "FREE" : "ALIVE"
    );
}

void print_both(cmcb* c, lmcb* l, char* msg)
{
  puts(msg);
  print_cmcb(c);
  print_lmcb(l);
}

void show_mem_state()
{
  void *curr_blk = fma;
  unsigned int covered = 0;

  while(covered < (HEAP_SIZE + MCB_PADDING))
  {
    covered += ASCMCB(curr_blk)->size + MCB_PADDING;
    printf("%s : %x : %s\n",ASCMCB(curr_blk)->type == FREE ? "FCMCB" : "ACMCB", curr_blk, (ASCMCB(curr_blk)->karen != NULL) ? ASCMCB(curr_blk)->karen->process_name : "NULL");
    printf("%d\n",ASCMCB(curr_blk)->size);
    curr_blk += sizeof(cmcb) + ASCMCB(curr_blk)->size;
    printf("%s\n",ASLMCB(curr_blk)->type == FREE ? "FLMCB" : "ALMCB");
    curr_blk += sizeof(lmcb);
  }

  puts("END OF HEAP");
}