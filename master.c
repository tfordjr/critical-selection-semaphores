// Terry Ford Jr - CS 4760 - Project 2 - 09/17/23


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <stdbool.h>
#include <time.h>
#include <sys/mman.h>
#include <semaphore.h>
#include <fcntl.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <sys/wait.h>
#include "config.h"

#define SHM_NAME "/shared_memory"
#define SHM_SIZE 1

void help();
void sigCatch(int);
void timeout(int);
void logfile();

int main(int argc, char** argv){
	int option;   
  	int numChildren; 
   	if (argc < 3) {   //  Helps to prevent users from providing too few arguments
		perror("master: Error: Not enough arguments provided\n");
		return 0;
	}

        while ( (option = getopt(argc, argv, "t:")) != -1) {    // Only one arg: t- 
                switch(option) {				
                  	case 't':                    
		    		for (int i = 1; i < argc; i++) {   //  cycles through args to find -t seconds 
       			 		if (strcmp(argv[i], "-t") == 0 && i + 1 < argc) {
            		 			timeoutSeconds = atoi(argv[i + 1]);  // casts str to int
            	    	 			break;
        	         		}
		    		}			   	 
	  	  	default:
		  		for (int i = 1; i < argc; i++) {    // Cycles to find final numChildren arg
					if (strcmp(argv[i], "-t") == 0 && i + 2 < argc) {
                        			if (atoi(argv[i + 2]) > maxChildren){
							numChildren = 20;//  Does not allow more than 20 children
							printf("Too many children provided! ");
							printf("Proceeding with %d children\n", maxChildren);
						} else {
							numChildren = atoi(argv[i + 2]);    
						}					
					break;
                        		} 
		  		}
		}
	}

	// Allocates shared memory boolean lock. lock will ensure one child works at a time on cstest
	int shmid = shm_open(SHM_NAME, O_CREAT | O_RDWR, 0666);   //  Creating shared memory
	if (shmid == -1) {
		perror("master: Error: shmget failed\n");
		exit(0);
	}

	int trunc = ftruncate(shmid, SHM_SIZE);
	if (trunc == -1){
		perror("master: Error: truncation failed\n");
		exit(0);
	}

	bool* lock = (bool*)mmap(NULL, SHM_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, shmid, 0); 
	if (lock == MAP_FAILED) {
		perror("master: Error: mmat failed\n");
		exit(0);
	}

	*lock = 0; // Initializing lock to the 'unlocked' position so children can gain access to critical section
	




	signal(SIGINT, sigCatch);
	signal(SIGALRM, timeout);
	alarm(timeoutSeconds);

	for (int i = 1; i < (numChildren + 1); i++) {
	
		pid_t childPid = fork();  // Create Child here

		if (childPid == 0 ) { // Children perform the following	
		 	if(execl("./slave","slave", (char *)NULL) == -1) {
				perror("Child execution of slave failed ");				
			}	
			exit(0);
				
		} else 	if (childPid == -1) {  // Error message for failed fork (child has PID -1)
            		perror("master: Error: Fork has failed!");
            		return 1;
        	}          
			
		//wait(NULL);  // This Assures children perform in order
    	}
	
	for (int i = 0; i < numChildren; i++) { // Only the parent should reach here and wait for children
		wait(NULL);	// Parent Waiting for children
	}

	printf("Child processes have completed.\n");
	shmdt(lock);

	logfile();
	return 0;	
}
            
void sigCatch(int signum) {
	printf("Cannot end program with Ctrl+C");
}

void timeout(int signum){
	printf("Timeout has occured. Now terminating all child processes.\n ");
	logfile();
	kill(0, SIGKILL);
}

void logfile(){
        char t[9];
        strftime(t, sizeof(t), "%T", localtime(&(time_t){time(NULL)}));

        char outfile[22];
        snprintf(outfile, sizeof(outfile), "logfile.master.%d", getpid());

        FILE* outputfile = fopen(outfile, "a");
        fprintf(outputfile, "%s Master process terminated - master(parent) pid: %d\n", t, getpid());
        fclose(outputfile);
}


