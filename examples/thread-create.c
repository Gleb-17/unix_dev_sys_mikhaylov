#include <pthread.h>
#include <stdio.h>
void* thr_func(void* arg) {
	while(1) fputc('c',stdout);
	return NULL;
}
int main() {
	pthread_t thr_id;
	pthread_create(&thr_id, NULL, &thr_func,NULL);
	while(1) fputc('p',stdout);
	return 0;
}
