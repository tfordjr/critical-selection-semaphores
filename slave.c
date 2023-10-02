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

#define SHM_NAME "/shared_memory"
#define SHM_SIZE 1
#define SEM_NAME "/file_semaphore"

void logfile(sem_t* file_semaphore);

int main(){
	//time_t t = time(NULL);
	char t[9];
	strftime(t, sizeof(t), "%T", localtime(&(time_t){time(NULL)}));

	char* outfile = "cstest";
	FILE* outputfile = fopen(outfile, "a");
	if (!outputfile) {   // perror file open failure
                perror("slave: Error: File open operation failed!");
                exit(0);
        }

	srand(time(NULL));
	int randomDelay = rand() % 3 + 1;
	

	

	
	int shmid = shm_open(SHM_NAME, O_RDWR, 0);    //Creating shared memory
        if (shmid == -1) {
                perror("slave: Error: shm_open failed\n");
                exit(0);
        }

        bool* lock = mmap(NULL, SHM_SIZE, PROT_READ, MAP_SHARED, shmid, 0);            // Attaching Shared Memory
        if (lock == NULL) {
                perror("slave: Error: mmap failed\n");
                exit(0);
        }
	

	sem_t* file_semaphore = sem_open(SEM_NAME, O_CREAT, 0666, 1);
	if (file_semaphore == SEM_FAILED){
		perror("slave: Error: sem_open failed\n");
		exit(0);
	}




	
	sleep(randomDelay);	
		
	if (sem_wait(file_semaphore) == -1) {
		perror("slave: Error: sem_wait failed\n");
		exit(0);
	}	

	fprintf(outputfile, "%s File modified by process number %d\n", t, getpid());
	fflush(outputfile);

	if (sem_post(file_semaphore) == -1){
		perror("slave: Error: sem_post failed");
		exit(0);
	}
		
	logfile(file_semaphore);
	sleep(randomDelay);
	
	



	fclose(outputfile);
	
	sem_close(file_semaphore);
	sem_unlink(SEM_NAME);

	shmdt(lock);
	return 0;
}

void logfile(sem_t* file_semaphore){
	char t[9];
	strftime(t, sizeof(t), "%T", localtime(&(time_t){time(NULL)}));
	
	char outfile[16];
	snprintf(outfile, sizeof(outfile), "logfile.%d", getpid());

	FILE* outputfile = fopen(outfile, "a");

	if(sem_wait(file_semaphore) == -1){
		perror("slave: Error: sem_wait failed\n");
		exit(0);
	}
	
	fprintf(outputfile, "%s File modified by process number %d\n", t, getpid());
	fflush(outputfile);
	
	if (sem_post(file_semaphore) == -1){
		perror("slave: Error: sem_post failed\n");
		exit(0);
	}
	
	fclose(outputfile);
}
