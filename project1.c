// Terry Ford Jr - CS 4760 - Project 1 - 08/29/23

void help();

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>


int main(int argc, char** argv){
   int option;
   char* infile = "input.dat";
   char* outfile = "output.dat";
        while ( (option = getopt(argc, argv, "hi:o:")) != -1) {
                switch(option) {
                  case 'h':
                    help();
                    return 0;
                  case 'i':
                    printf("i...\n");
		    for (int i = 1; i < argc; i++) {
       			 if (strcmp(argv[i], "-i") == 0 && i + 1 < argc) {
            		 infile = argv[i + 1];
            	    	 break;
        	         }
		    }	
		    break;
	  	  case 'o':
                    printf("o...\n");
		    for (int i = 1; i < argc; i++) {
                         if (strcmp(argv[i], "-o") == 0 && i + 1 < argc) {
                         outfile = argv[i + 1];
                         break;
                         }
		    }
		  }
	}
	
	printf("Input filename is: %s\n", infile);
	printf("Output filename is: %s\n", outfile);

	FILE* inputfile = fopen(infile, "r");
	FILE* outputfile = fopen(outfile, "w");

	int numChildren;
	char buffer[256];
	if (fgets(buffer, sizeof(buffer), inputfile) != NULL) {
		numChildren = (int)*buffer;
		printf("Firstline of file: %s", buffer);
		printf("Converted to int: %c\n", numChildren);
	} else {
		printf("File empty.\n");
	}	
	
	
	for(int i=0; i < numChildren; i++){
		
 		pid_t childPid = fork();
        	if (childPid == 0){
           		printf("I am the child with PID %d and parent PID %d\n", getpid(), getppid());
        	}

		return 0;
		
	}
	
	fclose(inputfile);
	
}
            
void help(){
        printf("-h detecteed. Printing Help Message...\n");
        printf("The options for this program are: \n");
        printf("-h Help feature. This takes no arguments.\n");
        printf("-i The argument following -i will be the input file. Required.\n");
        printf("-o The argument following -o will be the output file. Optional.\n");
}


