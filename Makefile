GCC = gcc
CFLAGS = -g -Wall -Wshadow

project1: reverse.o
	$(GCC) $(CFLAGS) reverse.o -o reverse

project1.o: reverse.c 
	$(GCC) $(CFLAGS) -c reverse.c

