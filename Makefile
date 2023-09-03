GCC = gcc
CFLAGS = -g -Wall -Wshadow

reverse: reverse.o
	$(GCC) $(CFLAGS) reverse.o -o reverse

%.o: %.c 
	$(GCC) $(CFLAGS) -c $<

