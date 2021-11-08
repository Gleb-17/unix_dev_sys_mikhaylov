#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h> 
#include <string.h>
#include <errno.h>

const char *fifoname = "myfifo"; 
const char *data = "Data String for Output";
int main() {
	int fd;
	if (mkfifo(fifoname, 0660) == -1 ) 
		if ( errno != EEXIST ) {
			perror(fifoname);
			return 1;
		}
	if ((fd = open(fifoname, O_WRONLY)) == -1) {
		perror(fifoname);
		return 1;
	}
	write(fd,data,strlen(data));
	write(fd,"\n",1);
	close(fd);
	return 0;
}
