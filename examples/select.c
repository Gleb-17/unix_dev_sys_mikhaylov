#include <stdio.h>
#include <stdlib.h>
#include <sys/select.h>
#include <unistd.h>
#include <fcntl.h>

void report(int fd) {
	char buf[100];
	printf("FD %d is ready to read\n", fd);
	int bytes = read(fd, buf, 100);
	buf[bytes] = 0;
	printf("Got %d bytes from %d : %s\n", bytes, fd, buf);
}

int main() {
	int fifo1 = open("./fifo1", O_RDWR);
	int fifo2 = open("./fifo2", O_RDWR);
	if ((fifo1 == -1 ) || (fifo2 == -1)) {
		perror("fifo1,fifo2");
		exit(1);
	}
	printf("FIFO desc: %d %d\n", fifo1, fifo2);
	fd_set read_set;
	while (1) {
		FD_ZERO(&read_set);
		FD_SET(fifo1, &read_set);
		FD_SET(fifo2, &read_set);
		int res = select(fifo2+1, &read_set, NULL, NULL, NULL);
		if (res) {
      			if ( FD_ISSET(fifo1, &read_set) ) report(fifo1);
      			if ( FD_ISSET(fifo2, &read_set) )report(fifo2);
    		}
	}
	close(fifo1);
	close(fifo2);
	return 0;
}
