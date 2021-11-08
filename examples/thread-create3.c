#include <pthread.h>
#include <stdio.h>
struct thr_params {
	char c;
	int count;
};
void* thr_func(void* arg) {
	struct thr_params *p = (struct thr_params*)arg;
	int i;
	for (i=0; i < p->count; ++i)
		fputc(p->c,stdout);
	return (void *)i;
}
int main() {
	pthread_t thr1_id, thr2_id;
	int thread_ret;
	struct thr_params thr1_params;
	thr1_params.count = 1000;
	thr1_params.c = 'a';
	pthread_create(&thr1_id, NULL, &thr_func, &thr1_params);
	struct thr_params thr2_params;
	thr2_params.count = 1500;
	thr2_params.c = 'b';
	pthread_create(&thr2_id, NULL, &thr_func, &thr2_params);
	pthread_join(thr1_id, (void *)&thread_ret);
	printf("Thread %lu retuns %d\n", thr1_id, thread_ret);
	pthread_join(thr2_id, (void *)&thread_ret);
	printf("Thread %lu retuns %d\n", thr2_id, thread_ret);
	return 0;
}
