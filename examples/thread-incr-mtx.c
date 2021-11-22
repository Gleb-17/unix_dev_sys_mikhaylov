#include <pthread.h>
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>

int global = 0;
pthread_mutex_t mtx = PTHREAD_MUTEX_INITIALIZER;

int pr_exit(char const* str) {
        perror(str);
        exit(errno);
}

void* thr_func(void* arg) {
	int loops = *((int *) arg);
	int local;
	for (int i = 0; i < loops; i++) {
		/* global++; */
		if (pthread_mutex_lock(&mtx) != 0)
			pr_exit("pthread_mutex_lock");
		local = global;
		local++;
		global = local;
		if (pthread_mutex_unlock(&mtx)!=0)
			pr_exit("pthread_mutex_unlock");

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
