#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>

#define MAX_LINE 100

const char *string = "test data\n";

int main() {
	pid_t  pid;
	int num, fd_u[2], fd_d[2];
	char line[MAX_LINE];
	if ((pipe(fd_u) == -1) || (pipe(fd_d) == -1) ) {
		perror("pipe()");
		exit(1);
	}
	if ((pid = fork()) == -1) {
		perror("fork()");
		exit(1);
	}
	if (pid > 0) { // в родителе
		close(fd_u[0]); // закрываем выход fd_u, будем писать на его вход
		close(fd_d[1]); // закрываем вход fd_d, будем с него читать
		write(fd_u[1],string,strlen(string));
		close(fd_u[1]);
		num = read(fd_d[0],line,MAX_LINE);
		write(1, line, num);
		close(fd_d[0]);
		waitpid(pid, NULL, 0);
	} else { // в ребенке
		close(fd_u[1]); // закрываем вход fd_u, будем с него читать
		dup2(fd_u[0], STDIN_FILENO);
		close(fd_d[0]); // закрываем выход fd_d, будем в него писать
		dup2(fd_d[1], STDOUT_FILENO);
		execlp("tr", "tr", "[a-z]", "[A-Z]", NULL);
	}
	return 0;
}
