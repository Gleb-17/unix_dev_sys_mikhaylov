#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
int spawn(char *prog, char** args) {
	pid_t child_pid;
	child_pid = fork();
	if (child_pid != 0)
		return child_pid;
	else {
		execvp(prog,args);
		fprintf(stderr, "Error launching program %s\n",prog);
		exit(1);
	}
}
int main() {
	int child_status;
	char* args[] = { "ls", "-l", "/", NULL };
	spawn("ls", args);
	wait(&child_status);
	if (WIFEXITED(child_status))
		printf("Normal exit, code %d\n",WEXITSTATUS(child_status));
	else
		printf("Abnormal exit\n");
	printf("main() finished\n");
	return 0;
}

