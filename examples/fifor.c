#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h> 
#include <string.h>
#include <errno.h>

const char *fifoname = "myfifo"; 
char buffer[256];
int main() {
	FILE *fifo;
	if (mkfifo(fifoname, 0660) == -1 ) 
		if ( errno != EEXIST ) {
			perror(fifoname);
			return 1;
		}
	if ((fifo = fopen(fifoname, "r")) == NULL) {
		perror(fifoname);
		return 1;
	}
	fscanf(fifo, "%s", buffer);
	fclose(fifo);
	return 0;
}
