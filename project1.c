// Terry Ford Jr - CS 4760 - Project 1 - 08/29/23
// Git repo: https://github.com/tfordjr/reverse-input

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
   char* infile = "input.dat";  // Not file objects, just strings for filenames
   char* outfile = "output.dat";
        while ( (option = getopt(argc, argv, "hi:o:")) != -1) {  // getopt h no args, i.o req args
                switch(option) {
                  case 'h':
                    help();
                    return 0;     // terminates if -h is present
                  case 'i':
                    printf("i...\n");
		    for (int i = 1; i < argc; i++) {    // cycles through args 
       			 if (strcmp(argv[i], "-i") == 0 && i + 1 < argc) {
            		 infile = argv[i + 1];      // assigns arg follwing -i to string variable infile
            	    	 break;
        	         }
		    }	
		    break;
	  	  case 'o':
                    printf("o...\n");
		    for (int i = 1; i < argc; i++) {  // Same process as above for -o
                         if (strcmp(argv[i], "-o") == 0 && i + 1 < argc) {
                         outfile = argv[i + 1];
                         break;
                         }
		    }
		  }
	}
	
	printf("Input filename is: %s\n", infile);   // Prints accepted filenames
	printf("Output filename is: %s\n", outfile);

	FILE* inputfile = fopen(infile, "r");
	FILE* outputfilew = fopen(outfile, "w");  // w upsed here to overwrite ouput file contents 

	int numChildren;
	char buffer[256];  // string used with fgets throughout the program
	if (fgets(buffer, sizeof(buffer), inputfile) != NULL) {  // here fgets only enters first line of file
		
		numChildren = atoi(buffer);  // cast char* to int
		printf("Number of children: %d\n", numChildren); 

	} else {
		printf("File empty.\n");
	}	

	fprintf(outputfilew, "");  // parent process overwrites outputfile
	fclose(outputfilew);    
	FILE* outputfile = fopen(outfile, "a");	// open again in "a" so each child can append and not overwrite
	pid_t pids[numChildren];  // Array of child pids

	for (int i = 1; i < (numChildren + 1); i++) {
	
		pid_t childPid = fork();  // Create Child here
		int lineNumber = 0;

		if (childPid == 0 ) { // Children perform the following	
					
			while (fgets(buffer, sizeof(buffer), inputfile) != NULL) { 
				lineNumber++;       // Each child loops through every line of file
				int arraySize;      
				if (lineNumber == (2*i)-1) {   
					arraySize = atoi(buffer);   // first of pair is arraySize
					//fprintf(outputfile, "%d\n", arraySize);				
				} else if (lineNumber == (2*i)) {  // Second of pair is array to be reversed
					int arr[arraySize];
					int count = 0;
					
					char* token = strtok(buffer, " ");   // Each in separated by " " in string 
					while (token != NULL && count < arraySize) {
						arr[count] = atoi(token);  // Add each into to int array
						token = strtok(NULL, " ");
						count++;
					}
					
					reverse(arr, arraySize); // reversing array (pass by reference)

					fprintf(outputfile, "%d: ", getpid());
					for (int i = 0; i < count; i++) { // loop though array
						fprintf(outputfile, "%d ", arr[i]);  // write each element to file
					}
					fprintf(outputfile, "\n");

				}					
			}
				
			fclose(inputfile);   // Closing both files
			exit(0);            // Each child ends here
		} else 	if (childPid == -1) {  // Error message for failed fork (child has PID -1)
            		perror("fork");
            		return 1;
        	}          
		pids[i-1] = childPid;	
		wait(NULL);  // This Assures children perform in order
    	}
	
	for (int i = 0; i < numChildren; i++) { // Only the parent should reach here and wait for children
		wait(NULL);	// Parent Waiting for children
	}

	printf("Child processes have completed.\n");
	fprintf(outputfile, "All children were: ");
	for (int i = 0; i < numChildren; i++) {   
		fprintf(outputfile, "%d ", pids[i]);  // Writing child pids
	}
	fprintf(outputfile, "\nParent PID: %d\n", getpid());
	fclose(outputfile);
	return 0;	
}
            
void help(){   // Help message here
        printf("-h detecteed. Printing Help Message...\n");
        printf("The options for this program are: \n");
        printf("\t-h Help feature. This takes no arguments.\n");
        printf("\t-i The argument following -i will be the input file. Optional - defaults to input.dat.\n");
        printf("\t-o The argument following -o will be the output file. Optional - defaults to output.dat.\n");
}

void reverse(int arr[], int size){   // int Array reversing function 
	for (int i = 0; i < size/2; i++) {
		int temp = arr[i];
		arr[i] = arr[size -(i+1)];
		arr[size -(i+1)] = temp;
	}
}
