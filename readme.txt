Terry Ford Project 1 - reverse 09/03/2023
Git: https://github.com/tfordjr/reverse-input

How to run my program:
make command will generate executable named reverse
./reverse command with optional -h, -i, and -o args will run the program.

Implementation:
two main files are: reverse.c and stack.h
reverse.c holds main() and help()
stack.h holds basic stack operations isEmpty(), isFull(), pop(), and push()

Also included are input files bigdata.dat, in.dat, and input.dat each with different inputs
And also the Makefile, which uses pattern rules

Notable inclusions are:
perror() messages for each file open operation and for failed forks
input and output files default to input.dat and output.dat respectively, will not change unless specified w/ -i, -o
Child process implementation
stack implementation for array reversing
version control with github
Makefile with pattern rules
Appropriate comments that briefly explain implementation and methods used

Notable exclusions are:
Does not conform precisely to input/output file defaults (specifications confilct on pages 1 and 2)
  Page 1: "If input and output filenames are not specified, the defaults should be input.dat and output.dat."
  Page 2: "If the user does not specify an input file, print the usage message and terminate. If the user does not specify an output file, create the filename by appending .out to the input file name."
Program assumes that the input file is always in the correct format ie. number of child processes or array sizes correct. 
