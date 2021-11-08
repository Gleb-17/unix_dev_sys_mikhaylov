#include <stdio.h>
#include <unistd.h>
#include <sys/shm.h>
#include <stdlib.h>
#include <error.h>

int shm_id = 0;

int main() {
	int *share;
	if (( shm_id = shmget (7890, getpagesize(), 0)) == -1) {
		perror("shmget()");
		exit(1);
	}
	if ((share = shmat(shm_id, NULL, SHM_RND|SHM_RDONLY)) == (void *)-1 ) {
		perror("shmat()");
		exit(1);
	}
	printf("Got SHMEM segment %d attached at: %p\n",shm_id, share);
	for (int i=0; i < 30; i++){
		sleep(1);
		printf("Got value %d\n", *share);
	}
	shmdt(share);
	return 0;
}
