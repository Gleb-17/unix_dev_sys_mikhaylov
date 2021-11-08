#include <sys/socket.h>
#include <sys/un.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>

#define MAXBUFSIZE 256
char buf[MAXBUFSIZE];
char tmp_file[L_tmpnam];

char const *msg = "Message from client";
char const *socket_file = "./usock";

int pr_exit(char const* str) {
	perror(str);
	exit(errno);
}

int main() {
	struct sockaddr_un srv_addr,cln_addr;
	int sockfd, saddrlen, caddrlen;
	if ((sockfd = socket(AF_UNIX,SOCK_DGRAM,0)) < 0) pr_exit("socket()");
	// адрес сокета сервера
	memset(&srv_addr, 0, sizeof(srv_addr));
	srv_addr.sun_family = AF_UNIX;
	strncpy(srv_addr.sun_path, socket_file, sizeof(socket_file));
	saddrlen = sizeof(srv_addr.sun_family) + sizeof(srv_addr.sun_path);
	// адрес сокета клиента
	memset(&cln_addr, 0, sizeof(cln_addr));
	cln_addr.sun_family = AF_UNIX;
	memset(tmp_file, 0, L_tmpnam);
	if (tmpnam(tmp_file) == NULL ) pr_exit("tmpnam()"); 
	strncpy(cln_addr.sun_path, tmp_file, sizeof(tmp_file));
	caddrlen = sizeof(cln_addr.sun_family) + sizeof(cln_addr.sun_path);
	if (bind(sockfd, (struct sockaddr *) &cln_addr,caddrlen) < 0 ) pr_exit("bind()");
	int msglen = strlen(msg);
	if (sendto(sockfd, msg, msglen, 0, (struct sockaddr *)&srv_addr, saddrlen) != msglen ) pr_exit("sendto");
	if (recv(sockfd, buf, MAXBUFSIZE, 0) < 0 ) pr_exit("recv()");
	write(1,tmp_file,sizeof(tmp_file));
	write(1, " - ",3);
	write(1,buf,sizeof(buf));
	close(sockfd);
	unlink(tmp_file);
	return 0;
}

