#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>

int main() {
	pid_t child_pid;
	printf("*** Before fork *** PID: %d\n",(int)getpid());
	child_pid = fork();
	if (child_pid != 0)
		printf("In parent. My PID: %d Child PID: %d\n",(int)getpid(),(int)child_pid);
	else
		printf("In child. PID: %d PPID: %d\n",(int)getpid(),(int)getppid());
	return 0;
}
