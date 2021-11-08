#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <time.h>

int shm_id=0;
int main () {
	int *share, num;
	srand(time(NULL));
	if ((shm_id = shmget (7890, getpagesize(), IPC_CREAT|IPC_EXCL|S_IRUSR|S_IWUSR)) == -1 ) {
		perror("shmget()");
		exit(1);
	}
	if ((share = shmat(shm_id, NULL, SHM_RND)) == (void *)-1 ) {
		perror("shmat()");
		exit(1);
	}
	printf("Got SHMEM segment %d attached at: %p\n",shm_id, share);
	for (int i=0; i < 30; i++) {
		num = random() % 1000;
		*share = num;
		printf("The next random number %d\n", num);
		sleep(1);
	}
	shmdt(share);
	shmctl(shm_id, IPC_RMID,0);
	return 0;
}
