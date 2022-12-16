/* getmem.c
   implements getmem (malloc) for memory system
   CSE 374 HW6
*/

#include <assert.h>
#include "mem.h"
#include "mem_impl.h"

/* This code will be filled in by the students.  Your primary job is to
   implement getmem */

/* initialize global variables ?*/

uintptr_t totalmalloc = BIGCHUNK;
freeNode* list = NULL;
const int SIZE = 16;

/* Are there helper functions that only getmeme will need?  Declare here. */

void* getmem(uintptr_t size);

/* Define your functions below: */

void* getmem(uintptr_t size) {
  /* make sure you return a pointer to the usable memory that
     is at least 'size' bytes long.
     To get you started we are 'stubbing in' a call that will
     return a usable value.  You will replace this code. */

  // if the size of sotrage for given pointer is 0
  // return NULL
  if (size <= 0) {
    return NULL;
  } 

  // initialize the free list
  if (list == NULL) {
    list = malloc(BIGCHUNK);
    list -> next = NULL;
    list -> size = 16000-16;
  }

  void* pointer = NULL;
  freeNode* previous = NULL;
  freeNode* current = list;
  const int MEMOSIZE = (size / 16 + 1) * 16;
  const int INCREASE = 8000;

  // reach the end of the list
  while (current->size < MEMOSIZE && current->next != NULL) {
    previous = current;
    current = current->next;
  }

  // if the end of the list has no enough space that is greater than 32
  // increase the size of the space
  if (current -> size < MEMOSIZE + 32 && current -> next == NULL) {
    freeNode* more = malloc(INCREASE + MEMOSIZE);
    totalmalloc = totalmalloc + (uintptr_t)(INCREASE + MEMOSIZE);
    more -> next = NULL;
    more -> size = INCREASE + MEMOSIZE - SIZE;
    current -> next = more;
  }

  // get the remain space in the list
  int space = current -> size - MEMOSIZE;
  
  // if the remain space is greater than 32
  // split it
  if (space >= 32) {
    freeNode* split = (uintptr_t)current + SIZE + MEMOSIZE;
    split -> size = current -> size - SIZE - MEMOSIZE;
    split -> next = current -> next;
    
    pointer = (uintptr_t)current + SIZE;
    current -> size = MEMOSIZE;
    
    if (previous == NULL) {
      list = split;
    } else {
      previous -> next = split;
    }
  } else {
    // if the remain space is less than 32
    // add it
    pointer = (uintptr_t)current + SIZE;
    
    if (previous == NULL) {
      list = current -> next;
    } else {
      previous -> next = current -> next;
    }
  }

  return pointer;
}

