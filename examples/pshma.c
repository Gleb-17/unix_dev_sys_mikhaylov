#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <fcntl.h>
#include <time.h>

int shmfd=0;
#define PSHM_SIZE 100000000
char const *shm_name="shm0";

int f_error(const char *string) {
	perror(string);
	if (shmfd > 0)
		shm_unlink(shm_name);
	exit(1);
}

int main () {
	int num;
	srand(time(NULL));
	if ((shmfd = shm_open (shm_name, O_CREAT|O_EXCL|O_RDWR, 0600)) == -1 )
		f_error("shm_open()");
	printf("Created POSIX SHMEM with descriptor %d\n",shmfd);
	if (ftruncate(shmfd, PSHM_SIZE ) == -1 ) 
		f_error("ftruncate()");
	struct stat statbuf;
        if (fstat(shmfd, &statbuf) < 0) 
                f_error("fstat()");
	printf("SHMEM file size is %lu\n", statbuf.st_size);
	int *mmf_ptr;
	if ((mmf_ptr = (int *)mmap(0, PSHM_SIZE, PROT_WRITE, MAP_SHARED, shmfd, 0)) == MAP_FAILED) 
                f_error("mmap()");
	printf("SHMEM is mmapped to address: %p\n", mmf_ptr);
	for (int i=0; i < 30; i++) {
		num = random() % 1000;
		*mmf_ptr = num;
		printf("The next random number %d\n", num);
		sleep(1);
	}
	munmap(mmf_ptr, PSHM_SIZE);
	close(shmfd);
	shm_unlink(shm_name);
	return 0;
}
