#include <pthread.h>
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>

int global = 0;

int pr_exit(char const* str) {
        perror(str);
        exit(errno);
}

void* thr_func(void* arg) {
	int loops = *((int *) arg);
	int local;
	for (int i = 0; i < loops; i++) {
		/* global++; */
		local = global;
		local++;
		global = local;

	}
	return NULL;
}
int main(int argc, char *argv[]) {
	pthread_t thr_id1, thr_id2;
	int loops;
	if (argc > 1)
		loops = atoi(argv[1]); 
	else
		loops = 1000;
	printf("Loops: %d\n", loops);
	if (pthread_create(&thr_id1, NULL, &thr_func, &loops) !=0)
		pr_exit("pthread_create");
	if (pthread_create(&thr_id2, NULL, &thr_func, &loops) !=0)
		pr_exit("pthread_create");
	if (pthread_join(thr_id1, NULL) !=0)
		pr_exit("pthrread_join");
	if (pthread_join(thr_id2, NULL) !=0)
		pr_exit("pthread_join");
	printf("Global var = %d\n", global);
	return 0;
}
