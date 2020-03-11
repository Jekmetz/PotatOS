#include "memory_wrangler.h"
#include <system.h>
#include <mem/heap.h>

void* fma;
cmcb* ffree;
cmcb* falive;

unsigned int remaining_free;

#define SUCCESS 1
#define FAILURE 0

#define NEXT_BLOCK(block) (cmcb*)((unsigned char*)block + block->size + MCB_PADDING)
#define GET_LMCB(block) (lmcb*)((unsigned char*)block + block->size + sizeof(cmcb))

typedef unsigned char* location;

void* init() {
  // Creating heap
  fma = (void*)kmalloc(HEAP_SIZE + MCB_PADDING);

  // Setting free block
  ffree = fma;
  falive = NULL;
  ffree->karen = NULL;
  ffree->size = HEAP_SIZE;
  ffree->type = FREE;

  // ending block
  lmcb* end = (lmcb*)(sizeof(cmcb) + HEAP_SIZE);
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
         (location)curr_blk <
             (location)fma + HEAP_SIZE + MCB_PADDING) {
    curr_blk = NEXT_BLOCK(curr_blk);
    if ((location)curr_blk >
        (location)fma + HEAP_SIZE + sizeof(cmcb)) {
      // TODO: run compaction
      curr_blk = ffree;
    }
  }

  // creating block
  unsigned int prev_size = curr_blk->size;
  curr_blk->karen = karen;
  curr_blk->type = ALIVE;
  curr_blk->size = size;

  lmcb* e_blk = (lmcb*)((location)curr_blk + size + sizeof(cmcb));
  e_blk->top = curr_blk;
  e_blk->type = ALIVE;

  // fixing the free block
  cmcb* nfree = (cmcb*)((location)e_blk + sizeof(lmcb));
  nfree->karen = NULL;
  nfree->size = prev_size - size;
  nfree->type = FREE;
  if (curr_blk == ffree) {
    ffree = nfree;
  }

  lmcb* fend = (lmcb*)((location)nfree + nfree->size + sizeof(cmcb));
  fend->top = nfree;
  fend->type = FREE;

  remaining_free -= size;

  return curr_blk + sizeof(cmcb);
}

int internal_free(void* data) {
  cmcb* block = data - sizeof(cmcb);
  lmcb* end = data + block->size;

  // Setting variables of current block
  block->type = FREE;
  end->type = FREE;
  block->karen = NULL;
  remaining_free += block->size;

  if (block == fma) {
    ffree = block;
  }

  // checking previous block
  lmcb* prev_lmcb = (lmcb*)((location)block - sizeof(lmcb));
  if ((location)prev_lmcb >= (location)fma && prev_lmcb->type == FREE) {
    prev_lmcb->top->size += block->size + MCB_PADDING;
    end->top = prev_lmcb->top;
    block = end->top;
  }

  // checking next block
  cmcb* next_cmcb = (cmcb*)((location)block + MCB_PADDING + block->size);
  if ((location)next_cmcb < (location)fma + HEAP_SIZE + MCB_PADDING && next_cmcb->type == FREE)
  {
    lmcb* next_lmcb = GET_LMCB(next_cmcb);
    next_lmcb->top = block;
    block->size += next_cmcb->size + MCB_PADDING;
  }

  return SUCCESS;
}