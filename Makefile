# Makefile for mem memory system, CSE374 22WI

CC = gcc
CARGS = -Wall -std=c11

all: bench

# basic build
bench: bench.o getmem.o freemem.o mem_utils.o
	$(CC) $(CARGS) -o bench $^

# The executable
$(TARGET): bench.o mem_utils.o memory.o
	$(CC) $(CFLAGS) -g -o bench bench.o mem_utils.o getmem.o freemem.o

# Individual source files
bench.o: bench.c mem.h mem_impl.h
	$(CC) $(CFLAGS) -g -c bench.c

mem_utils.o: mem_utils.c mem.h mem_impl.h
	$(CC) $(CFLAGS) -g -c mem_utils.c

getmem.o: getmem.c mem.h mem_impl.h
	$(CC) $(CFLAGS) -g -c getmem.c

freemem.o: freemem.c mem.h mem_impl.h
	$(CC) $(CFLAGS) -g -c freemem.c


test: bench
	./bench

# A "phony" target to remove built files and backups
clean:
	rm -f *.o bench *~
