#include "memory_wrangler.h"
#include <system.h>

void* fma;
cmcb* ffree;
cmcb* falive;

unsigned int remaining_free;

#define SUCCESS 1
#define FAILURE 0

void* init() {
  // Creating heap
  fma = kmalloc(HEAP_SIZE + MCB_PADDING);

  // Setting free block
  ffree = fma;
  falive = NULL;
  ffree->karen = NULL;
  ffree->size = HEAP_SIZE;
  ffree->type = FREE;

  // ending block
  lmcb* end = sizeof(cmcb) + HEAP_SIZE;
  end->top = ffree;
  end->type = FREE;

  remaining_free = HEAP_SIZE;

  return fma;
}

void* internal_malloc(unsigned int size, pcb_t* karen) {
  // checking that there is enough memory in the heap
  if (remaining_free < size) {
    return NULL;
  }

  // finding fisrt available free block

  cmcb* curr_blk = ffree;
  while ((curr_blk->size < size || curr_blk->type == ALIVE) &&
         curr_blk < fma + HEAP_SIZE + MCB_PADDING) {
    curr_blk = curr_blk->size + MCB_PADDING;
    if (curr_blk > fma + HEAP_SIZE + sizeof(cmcb)) {
      // TODO: run compaction
      curr_blk = ffree;
    }
  }

  // creating block
  unsigned int prev_size = curr_blk->size;
  curr_blk->karen = karen;
  curr_blk->type = ALIVE;
  curr_blk->size = size;

  lmcb* e_blk = curr_blk + size + sizeof(cmcb);
  e_blk->top = curr_blk;
  e_blk->type = ALIVE;

  // fixing the free block
  cmcb* nfree = e_blk + sizeof(lmcb);
  nfree->karen = NULL;
  nfree->size = prev_size - size;
  nfree->type = FREE;

  lmcb* fend = nfree + nfree->size + sizeof(cmcb);
  fend->top = nfree;
  fend->type = FREE;

  return curr_blk + sizeof(cmcb);
}

int internal_free(void* data) {
  cmcb* block = data - sizeof(cmcb);
  lmcb* end = data + block->size;

  // Setting variables of current block
  block->type = FREE;
  end->type = FREE;
  block->karen = NULL;

  if (block == fma)
  {
    ffree = block;
  }

  cmcb* nblock = block + MCB_PADDING;
  

  //TODO: combining free blocks
  return SUCCESS;
}