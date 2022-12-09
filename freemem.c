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
int checkAdj(freeNode* n1, freeNode* n2);
freeNode* makeNode(uintptr_t address, uintptr_t size, freeNode* next);

/* Define your functions below: */


/* Return the block of storage at location p to the pool of available free
   storage. The pointer value p must be one that was obtained as the result
   of a call to getmem. If p is NULL, then the call to freemem has no effect
   and returns immediately. */
void freemem(void* p) {
   if (p == NULL) {
      return;
   }
   uintptr_t address = (uintptr_t)p - NODESIZE;
   freeNode* addNode = (freeNode*) (address - NODESIZE);
   uintptr_t size = addNode->size;

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
   if (!address) {
      return NULL;
   }
   freeNode* new = (freeNode*) address;
   new->next = next;
   new->size = size;
   return new;
}

// Check if two nodes are adjacent.
int checkAdj(freeNode* n1, freeNode* n2) {
   uintptr_t n1_address = (uintptr_t)n1;
   uintptr_t n2_address = (uintptr_t)n2;
   uintptr_t n1_size = n1->size;
   uintptr_t n2_size = n2->size;
   // n1 is in the front of n2
   if (n1_address < n2_address) {
      if ((n1_address + NODESIZE + n1_size) == n2_address) {
         return 1;
      } else {
         return 0;
      }
   } 
   // n2 is in the front
   else { 
      if (n1_address == (n2_address + NODESIZE + n2_size)) {
         return 1;
      } else {
         return 0;
      }
   }
}
