// Terry Ford Jr - CS 4760 - Project 1 - 08/29/23


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

void help();
void reverse(int[], int);


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
	FILE* outputfilew = fopen(outfile, "w");

	int numChildren;
	char buffer[256];
	if (fgets(buffer, sizeof(buffer), inputfile) != NULL) {
		
		numChildren = atoi(buffer);
		printf("Number of children: %d\n", numChildren);

	} else {
		printf("File empty.\n");
	}	

	fprintf(outputfilew, "%d\n", numChildren);
	fclose(outputfilew);
	FILE* outputfile = fopen(outfile, "a");	

	// Children Created Here
	for (int i = 1; i < (numChildren + 1); i++) {
	
		pid_t childPid = fork();
		int lineNumber = 0;

		if (childPid == 0 ) {
			printf("I am child %d and I will access lines %d and %d\n", i, 2*i, (2*i)+1);
		
			while (fgets(buffer, sizeof(buffer), inputfile) != NULL) {
				lineNumber++;
				int arraySize;
				if (lineNumber == (2*i)-1) {
					arraySize = atoi(buffer);
					fprintf(outputfile, "%d\n", arraySize);					
				} else if (lineNumber == (2*i)) {
					printf("%s", buffer);
					int arr[arraySize];
					int count = 0;
					
					char* token = strtok(buffer, " ");
					while (token != NULL && count < arraySize) {
						arr[count] = atoi(token);
						token = strtok(NULL, " ");
						count++;
					}
					
					reverse(arr, arraySize);

					for (int i = 0; i < count; i++) {
						fprintf(outputfile, "%d ", arr[i]);
					}
					fprintf(outputfile, "\n");

				}
					
			}

				
			fclose(inputfile);
			fclose(outputfile);

			exit(0);
		} else 	if (childPid == -1) {
            		perror("fork");
            		return 1;
        	}  
        	 
    	}
	
	for (int i = 0; i < numChildren; i++) { // Only the parent should reach here and wait for children
		wait(NULL);
		//printf("Child Waiting (1 wait per child)...ppid() - %d\n", getppid());
	}

	printf("Child processes have completed.\n");
	

	return 0;	
}
            
void help(){
        printf("-h detecteed. Printing Help Message...\n");
        printf("The options for this program are: \n");
        printf("-h Help feature. This takes no arguments.\n");
        printf("-i The argument following -i will be the input file. Required.\n");
        printf("-o The argument following -o will be the output file. Optional.\n");
}

void reverse(int arr[], int size){
	for (int i = 0; i < size/2; i++) {
		int temp = arr[i];
		arr[i] = arr[size -1 -i];
		arr[size -1 -i] = temp;
	}
}
