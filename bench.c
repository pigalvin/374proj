/* bench.c is the benchmarking /test program for mem memory management
   bench.c is implemented as part of HW6, CSE374 22WI
*/

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <time.h>
#include "mem.h"

/* print_stats is a helper function that prints performance
   information about the memory system.  Requires a clock_t start
   time, and prints elapsed time plus results from get_mem_stats()
*/
void print_stats(clock_t start);

/* fill_mem is a helper function that takes in an address and the
   size of the memory allocation at that address.  It should fill
   the first 16 bytes (or up to size) with hexedecimal values.*/
void fill_mem(void* ptr, uintptr_t size);

/* Synopsis:   bench (main)
   [ntrials] (10000) getmem + freemem calls
   [pctget] (50) % of calls that are get mem
   [pctlarge] (10) % of calls requesting more memory than lower limit
   [small_limit] (200) largest size in bytes of small block
   [large_limit] (20000) largest size in byes of large block
   [random_seed] (time) initial seed for randn
*/
int main(int argc, char** argv ) {
  // Initialize the parameters
  int NTRIALS;
  int PCTGET;
  int PCTLARGE;
  int SMALL_L;
  int LARGE_L;

  (argc > 1) ? (NTRIALS = atoi(argv[1])) : (NTRIALS = 10000);
  (argc > 2) ? (PCTGET = atoi(argv[2])) : (PCTGET = 50);
  (argc > 3) ? (PCTLARGE = atoi(argv[3])) : (PCTLARGE = 10);
  (argc > 4) ? (SMALL_L = atoi(argv[4])) : (SMALL_L = 200);
  (argc > 5) ? (LARGE_L = atoi(argv[5])) : (LARGE_L = 20000);
  (argc > 6) ? srand(atoi(argv[6])) : srand(time(NULL));

  printf("Running bench for %d trials, %d%% getmem calls.\n", NTRIALS, PCTGET);

  void* blocks[NTRIALS];  // upperbound block storage
  int ntrials = 0, nblocks = 0;
  clock_t start = clock();


  // perform NTRIALS mem operations

  /* Students will fill in this section according to the assignment
     specs.*/
  while (ntrials < NTRIALS) { 
    if (!(rand() % 100 + 1) <= PCTGET) {
      if (nblocks > 0) {
        int index = rand() % nblocks;
        // move the last pointer
        // make the size smaller
        freemem(blocks[index]);
        blocks[index] = blocks[nblocks-1];
        nblocks--;
      }
    } else {
      void* location = NULL;
      
      // check if the usage is small
      if (!(rand() % 100 + 1) <= PCTLARGE) {
        location = getmem((uintptr_t)(rand() % SMALL_L) + 1);
        fill_mem(location, (uintptr_t)(rand() % SMALL_L + 1));
      } else {
        location = getmem((uintptr_t)(rand() % (LARGE_L - SMALL_L) + SMALL_L + 1));
        fill_mem(location, (uintptr_t)(rand() % (LARGE_L - SMALL_L) + SMALL_L + 1));
      }

      blocks[nblocks] = location;
      nblocks++;
    }
    ntrials++;
  }
  print_stats(start);
  return EXIT_SUCCESS;
}

/* Define helper functions here. */

void print_stats(clock_t start) {
  uintptr_t total_size = 0;
  uintptr_t total_free = 0;
  uintptr_t n_free_blocks = 0;
  clock_t end = clock();
  double time = ((double) (end - start)) / CLOCKS_PER_SEC;
  printf("Processor Time Used: %ds\n", time);
  get_mem_stats(&total_size, &total_free, &n_free_blocks);
  printf("Total Size: %d\n", total_size);
  printf("Total Free: %d\n", total_free);
  printf("Number Free Blocks: %d\n", n_free_blocks);
}

void fill_mem(void* startLoc, uintptr_t size) {
  if (size > 16)
    size = 16;

  for (int i = 0; i < size; i++) {
    unsigned char *filler = (unsigned char *)((uintptr_t)startLoc + i);
    filler = 0xFE;
  }
}
