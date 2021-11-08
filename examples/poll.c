#include <stdio.h>
#include <stdlib.h>
#include <poll.h>
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
	struct pollfd fds[2];  // будем отслеживать 2 канала
	fds[0].fd = open("./fifo1", O_RDWR);
	fds[0].events = POLLIN; // отслеживаем поступление данных
	fds[1].fd = open("./fifo2", O_RDWR);
	fds[1].events = POLLIN; // поступление данных
	if ((fds[0].fd == -1 ) || (fds[1].fd == -1)) {
		perror("fifo1,fifo2");
		exit(1);
	}
	printf("FIFO desc: %d %d\n", fds[0].fd, fds[1].fd);

	while (1) {
		int res = poll(fds, 2, 10000);
		if (res > 0 ) {
      			if ( fds[0].revents & POLLIN ) {
				report(fds[0].fd);
				fds[0].revents = 0;
			}
      			if ( fds[1].revents & POLLIN ) {
				report(fds[1].fd);
				fds[1].revents = 0;
			}

    		}
	}
	close(fds[0].fd);
	close(fds[1].fd);
	return 0;
}
