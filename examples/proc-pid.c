#include <stdio.h>
#include <unistd.h>
int main() {
	printf("PID: %d PPID: %d\n", (int)getpid(), (int)getppid());
	return 0;
}
