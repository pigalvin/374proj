/* freemem.c
   implements freemem (free) for memory system
   CSE 374 HW6
*/

#include <assert.h>
#include "mem.h"
#include "mem_impl.h"

/* This code will be filled in by the students.  Your primary job is to
   implement freemem functions.  */

/* initialize global variables? */
freeNode* list = NULL;

/* Are there helper functions that only freemem needs?  Declare here. */
void checkAdj(freeNode* n1, freeNode* n2);
freeNode* makeNode(uintptr_t address, uintptr_t size, freeNode* next);

/* Define your functions below: */


/* Return the block of storage at location p to the pool of available free
   storage. The pointer value p must be one that was obtained as the result
   of a call to getmem. If p is NULL, then the call to freemem has no effect
   and returns immediately. */
void freemem(void* p) {
   if (p == NULL) {
      return NULL;
   }
   uintptr_t address = (uintptr_t)p - NODESIZE;
   uintptr_t size = (freeNode*)(address - NODESIZE)->size;

   if (list == NULL) {
      list = makeNode(address, size, NULL);
   }
   freeNode* node = list;
   // insert in the front of list
   if (address < (uintptr_t)list) {
      freeNode* insertedNode = makeNode(address,size,list);
      list = insertedNode;
   } 
   //insert in the middle of list
   while (node->next) {
      if ((uintptr_t)node < address && (uintptr_t)node->next > address) {
         freeNode* insertedNode = makeNode(address,size,node->next);
         node->next = insertedNode;
      }
      node = node->next;
   }
   // insert in the end of list
   if (address > (uintptr_t)node) {
      freeNode* insertedNode = makeNode(address,size,NULL);
      node->next = insertedNode;
   }
   // Combines the adjacent blocks.
   node = list;
   while (node->next) {
      if (checkAdj(node, node->next)) {
         freeNode* temp = node->next;
         node->next = temp->next;
         node->size = node->size + temp->size + NODESIZE;
      } else {
         node = node->next;
      }
   }
}

// construct a new node with given size, address and the next node that
// it points to.
freeNode* makeNode(uintptr_t address, uintptr_t size, freeNode* next) {
   if (address == NULL) {
      return NULL;
   }
   freeNode* new = (freeNode*) address;
   new->next = next;
   new->size = size;
   return new;
}

// Check if two nodes are adjacent.
int checkAdj(freeNode* n1, freeNode* n2) {
   uintptr_t n1_size = n1->size;
   uintptr_t n2_size = n2->size;
   // n1 is in the front of n2
   if ((uintptr_t)n1 < (uintptr_t)n2) {
      return (uintptr_t)n1 + NODESIZE + n1_size == (uintptr_t)n2;
   } 
   // n2 is in the front
   else { 
      return (uintptr_t)n1 == (uintptr_t)n2 + NODESIZE + n2_size;
   }
}
