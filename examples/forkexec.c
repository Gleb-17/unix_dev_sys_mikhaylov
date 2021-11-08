#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
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
	char* args[] = { "ls", "-l", "/ake", NULL };
	spawn("ls", args);
	printf("main() finished\n");
	return 0;
}

