#include <stdio.h>
#include <stdlib.h>
#include <sys/epoll.h>
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
	int epollfd = epoll_create(1); // дескриптор epoll
	if (epollfd < 0) {
		perror("epoll_create");
		exit(1);
	}
	struct epoll_event ev[2]; 
	struct epoll_event pevents; // выбираем события по 1 за раз
	ev[0].events = EPOLLIN; // отслеживаем приход данных
	ev[1].events = EPOLLIN; // отслеживаем приход данных
	// инициализация структуры наблюдаемых дескрипторов
	ev[0].data.fd = fifo1; 
	// добавление дескриптора в список наблюдаемых
	if ( epoll_ctl( epollfd, EPOLL_CTL_ADD, fifo1, &ev[0] ) != 0 ) {
		perror("epoll_ctl fifo1");
		exit(1);
	}
	ev[1].data.fd = fifo2;
	if ( epoll_ctl( epollfd, EPOLL_CTL_ADD, fifo2, &ev[1] ) != 0 ) {
		perror("epoll_ctl fifo2");
		exit(1);
	}
	printf("FIFO desc: %d %d\n", fifo1, fifo2);

	while (1) {
		int res = epoll_wait(epollfd, &pevents, 1, -1); 
		if ( res > 0 ) report(pevents.data.fd);
	}
	close(fifo1);
	close(fifo2);
	return 0;
}
