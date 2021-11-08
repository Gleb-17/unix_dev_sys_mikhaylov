#include <pthread.h>
#include <stdio.h>

void* thr_func(void* arg) {
	while(1) fputc('c',stdout);
	return NULL;
}

int main() {
	pthread_t thr_id;
	pthread_attr_t attr;
	pthread_attr_init(&attr);
	pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
	pthread_create(&thr_id, &attr, &thr_func, NULL);
	pthread_attr_destroy(&attr);
	pthread_join(thr_id, NULL);
	return 0;
}
