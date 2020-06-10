#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/stat.h> 
#include <fcntl.h>           
#include <string.h>
#include <semaphore.h>

#define SHARED_MEM "/SHARED_MEM"
#define NUM_CALLS 5

struct shared_str {
	sem_t sem;
	size_t len;
	char buf[256];
};

void errExit(char* msg){
	perror(msg);
	exit(EXIT_FAILURE);
}

int main(){
	int fd = shm_open(SHARED_MEM, O_CREAT | O_RDWR, S_IRUSR);
	if (fd < 0){
		errExit("failed to open shared memory object");
	}
	if (ftruncate(fd, sizeof(struct shared_str))){
		errExit("failed to truncate");
	}
	struct shared_str *sstr = mmap(NULL, sizeof(struct shared_str),
			PROT_READ | PROT_WRITE,
			MAP_SHARED, fd, 0);
	if (sstr == MAP_FAILED)
		errExit("mmap");
	if (sem_init(&sstr->sem, 1, 0) == -1)
		errExit("sem_init-sem1");
	pid_t pid;
	if ((pid = fork()) < 0) {
		errExit("failed to fork");
	}
	if ( pid == 0 ){
		printf("child is running...\n");
		for (int i =0 ; i <NUM_CALLS ; i++) {
			sleep(1);
			sstr -> len = snprintf((char*)&sstr->buf,256,"hallo no. %d",i);
			if (sem_post(&sstr->sem) == -1)
				errExit("sem_post");
		}
		close(fd);
		exit(EXIT_SUCCESS);
	}
	for (int i =0 ; i<NUM_CALLS ; i++) {
		if (sem_wait(&sstr->sem) == -1)
			errExit("sem_wait");
		pread(fd,sstr, sizeof(struct shared_str),0);	
		printf("text in shared buffe: %s\n",sstr -> buf);
	}
	int status;
	pid = wait(&status);
	printf("child process %d terminated with status %d\n", pid, WEXITSTATUS(status));
	shm_unlink(SHARED_MEM);
	exit(EXIT_SUCCESS);
}
