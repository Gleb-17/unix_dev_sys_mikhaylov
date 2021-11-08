#include <sys/socket.h>
#include <sys/un.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>

#define MAXBUFSIZE 256

char buf[MAXBUFSIZE];
char const* socket_file = "./usock";

int pr_exit(char const* str) {
	perror(str);
	exit(errno);
}

int main() {
	struct sockaddr_un srv_addr;
	int sockfd, saddrlen;
	if ((sockfd = socket(AF_UNIX,SOCK_DGRAM,0)) < 0) pr_exit("socket()");
	unlink(socket_file);
	memset(&srv_addr, 0, sizeof(srv_addr));
	srv_addr.sun_family = AF_UNIX;
	strncpy(srv_addr.sun_path, socket_file, sizeof(socket_file));
	saddrlen = sizeof(srv_addr.sun_family) + sizeof(srv_addr.sun_path);
	if (bind(sockfd, (struct sockaddr *) &srv_addr,saddrlen) < 0 ) pr_exit("bind()");
	while (1) {
		if (recv(sockfd, buf, MAXBUFSIZE, 0) < 0 ) pr_exit("recv()");
		write(1,buf, sizeof(buf));
		write(1,"\n",1);
	}
	close(sockfd);
	return 0;
}

