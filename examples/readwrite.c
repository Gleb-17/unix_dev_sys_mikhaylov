#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>

const char *myfile = "index.html";

int main() {
	int fdin;
	struct stat statbuf;
	char *bufin;
	if ((fdin=open(myfile, O_RDONLY)) < 0) {
		perror(myfile);
		return 1;
	}
	if (fstat(fdin, &statbuf) < 0) {
		perror(myfile);
		return 1;
	}
	if ((bufin = (char *)malloc(statbuf.st_size)) == NULL) {
		fprintf(stderr, "Error alocating memory!\n");
		return 1;
	}
	if (read(fdin,bufin,statbuf.st_size) != statbuf.st_size) {
		perror(myfile);
		return 1;
	}
	if (write(1,bufin,statbuf.st_size) != statbuf.st_size) {
		perror("stdout");
		return 1;
	}
	free(bufin);
	close(fdin);
	return 0;
}
