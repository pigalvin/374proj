#include <assert.h>
#include "mem.h"
#include "mem_impl.h"

uintptr_t totalmalloc = 0;
freeNode* freelist = NULL;

/* helper functions mm_split will split a given block
   into two blocks */
void mm_split(freeNode* node, uintptr_t size);

void* getmem(uintptr_t size);

void* getmem(uintptr_t size) {
  if (size < 0) {
    return NULL;
  }
  
  if (size % 0xFF != 0) {
    size = (size / 0xFF) * 0xFF + 0xFF;
  }

  freeNode* nodeMem = (freeNode*) malloc(size + NODESIZE);
  totalmalloc = totalmalloc + size + NODESIZE;
  if (nodeMem == NULL) {
    freelist = NULL;
  } else {
    nodeMem -> size = size;
    nodeMem -> next = NULL;
    freelist = nodeMem;
  }

  freeNode* newNode = NULL;
  freeNode* current = freelist;

  while (current != NULL) {
    if (current -> size >= MINCHUNK) {
      newNode = current;
    }
    current = current -> next;
  } 

  // allocate new memory if size is too big
  if (size > newNode -> size || newNode == NULL) {
    newNode = (freeNode*)malloc(size + NODESIZE);
    totalmalloc += size + NODESIZE;
    newNode -> size = size;
  }

  // mm_split node if blocks are too large
  if (newNode -> size >= (size + NODESIZE + MINCHUNK)) {
    mm_split(newNode, size);
  }

  uintptr_t addr = (uintptr_t) newNode + NODESIZE;
  return (void*)addr;
}

// mm_split can mm_split a too large block into two smaller one
void mm_split(freeNode* node, uintptr_t size) {
  freeNode* newNode = (freeNode*)((uintptr_t)(node) + size + NODESIZE);
  newNode -> next = node -> next;
  newNode -> size = node -> size - (size + NODESIZE);
  node -> next = newNode;
  node -> size = size;
}

