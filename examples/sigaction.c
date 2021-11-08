#include <signal.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>

sig_atomic_t sig_count = 0;

void usr1_handler (int signum) {
	++sig_count;
}

int main () {
	struct sigaction sa;
	memset(&sa, 0, sizeof(sa));
	sa.sa_handler = &usr1_handler;
	sigaction(SIGUSR1, &sa, NULL);
	printf("Start counting SIGUSR1 in PID %d\n", (int)getpid());
	for (int i=0; i<100; i++) 
		sleep(1);
	printf("SIGUSR1 was raised %d times\n", (int)sig_count);
}

