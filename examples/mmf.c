#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <stdio.h>
#include <sys/mman.h>

const char *myfile = "index.html";

int main() {
	int fdin;
	struct stat statbuf;
	void *mmf_ptr;
	if ((fdin=open(myfile, O_RDONLY)) < 0) {
		perror(myfile);
		return 1;
	}
	if (fstat(fdin, &statbuf) < 0) {
		perror(myfile);
		return 1;
	}
	if ((mmf_ptr = mmap(0, statbuf.st_size, PROT_READ, MAP_SHARED, fdin, 0)) == MAP_FAILED) {
        	perror("myfile");
		return 1;
	}
	if (write(1,mmf_ptr,statbuf.st_size) != statbuf.st_size) {
		perror("stdout");
		return 1;
	}
	close(fdin);
	munmap(mmf_ptr,statbuf.st_size);
	return 0;
}
