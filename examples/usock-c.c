#include <sys/socket.h>
#include <sys/un.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>


char const *msg = "Message from client";
char const *socket_file = "./usock";

int pr_exit(char const* str) {
	perror(str);
	exit(errno);
}

int main() {
	struct sockaddr_un srv_addr;
	int sockfd, saddrlen;
	if ((sockfd = socket(AF_UNIX,SOCK_DGRAM,0)) < 0) pr_exit("socket()");
	memset(&srv_addr, 0, sizeof(srv_addr));
	srv_addr.sun_family = AF_UNIX;
	strncpy(srv_addr.sun_path, socket_file, sizeof(socket_file));
	saddrlen = sizeof(srv_addr.sun_family) + sizeof(srv_addr.sun_path);
	int msglen = strlen(msg);
	if (sendto(sockfd, msg, msglen, 0, (struct sockaddr *)&srv_addr, saddrlen) != msglen ) pr_exit("sendto");
	close(sockfd);
	return 0;
}

