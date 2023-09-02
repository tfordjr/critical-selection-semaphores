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

		if (childPid == 0 ) { // Children perform the following	
					
			while (fgets(buffer, sizeof(buffer), inputfile) != NULL) { 
				lineNumber++;       // Each child loops through every line of file
				int arraySize;      
				if (lineNumber == (2*i)-1) {  // Child interacts only with assigned data
					arraySize = atoi(buffer);   // Cast char* to int
					fprintf(outputfile, "%d\n", arraySize);					
				} else if (lineNumber == (2*i)) {					
					int arr[arraySize];
					int count = 0;
					
					char* token = strtok(buffer, " ");
					while (token != NULL && count < arraySize) {
						arr[count] = atoi(token);
						token = strtok(NULL, " ");
						count++;
					}
					
					reverse(arr, arraySize); // reversing array (pass by reference)

					for (int i = 0; i < count; i++) { // loop though array
						fprintf(outputfile, "%d ", arr[i]);  // write each element to file
					}
					fprintf(outputfile, "\n");

				}					
			}
				
			fclose(inputfile);   // Closing both files
			fclose(outputfile);
			exit(0);            // Each child ends here
		} else 	if (childPid == -1) {  // Error message for failed fork (child has PID -1)
            		perror("fork");
            		return 1;
        	}          	
		wait(NULL);  // This Assures children perform in order
    	}
	
	for (int i = 0; i < numChildren; i++) { // Only the parent should reach here and wait for children
		wait(NULL);	// Parent Waiting for children
	}

	printf("Child processes have completed.\n");
	return 0;	
}
            
void help(){
        printf("-h detecteed. Printing Help Message...\n");
        printf("The options for this program are: \n");
        printf("\t-h Help feature. This takes no arguments.\n");
        printf("\t-i The argument following -i will be the input file. Optional - defaults to input.dat.\n");
        printf("\t-o The argument following -o will be the output file. Optional - defaults to output.dat.\n");
}

void reverse(int arr[], int size){
	for (int i = 0; i < size/2; i++) {
		int temp = arr[i];
		arr[i] = arr[size -(i+1)];
		arr[size -(i+1)] = temp;
	}
}
