#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>
#include <time.h>
#include <string.h>
#include <sys/mman.h>
#include <semaphore.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>



#define SEM_NAME "/file_semaphore"

void logfile();

int main(){	
	char t[9];
	strftime(t, sizeof(t), "%T", localtime(&(time_t){time(NULL)}));

	char* outfile = "cstest";
	FILE* outputfile = fopen(outfile, "a");
	if (!outputfile) {   // perror file open failure
                perror("slave: Error: File open operation failed!");
                exit(0);
        }

	srand(getpid());
	int randomDelay = rand() % 3 + 1;

	sem_t* file_semaphore = sem_open(SEM_NAME, O_CREAT, 0666, 1);
	if (file_semaphore == SEM_FAILED){
		perror("slave: Error: sem_open failed\n");
		exit(0);
	}

	
	for (int i = 0; i < 5; i++) {

		if (sem_wait(file_semaphore) == -1) {
			perror("slave: Error: sem_wait failed\n");
			exit(0);
		}	
		
		sleep(randomDelay);
		strftime(t, sizeof(t), "%T", localtime(&(time_t){time(NULL)}));
		fprintf(outputfile, "%s File modified by process number %d\n", t, getpid());
		fflush(outputfile);


		if (sem_post(file_semaphore) == -1){
			perror("slave: Error: sem_post failed");
			exit(0);
		}
	
		logfile();
	}


	fclose(outputfile);
	
	sem_close(file_semaphore);
	sem_unlink(SEM_NAME);

	return 0;
}

void logfile(){
	char t[9];
	strftime(t, sizeof(t), "%T", localtime(&(time_t){time(NULL)}));
	
	char outfile[16];
	snprintf(outfile, sizeof(outfile), "logfile.%d", getpid());

	FILE* outputfile = fopen(outfile, "a");
	
	strftime(t, sizeof(t), "%T", localtime(&(time_t){time(NULL)}));
	fprintf(outputfile, "%s File modified by process number %d\n", t, getpid());
	fflush(outputfile);
	
	fclose(outputfile);
}
