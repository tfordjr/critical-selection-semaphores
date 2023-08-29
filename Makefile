GCC = gcc
CFLAGS = -g -Wall -Wshadow

project1: project1.o
	$(GCC) $(CFLAGS) project1.o -o project1

project1.o: project1.c 
	$(GCC) $(CFLAGS) -c project1.c

