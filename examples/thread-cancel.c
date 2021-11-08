#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
struct thr_params {
	char c;
	int count;
};
void* thr_func(void* arg) {
	pthread_setcanceltype(PTHREAD_CANCEL_DEFERRED, NULL);
	struct thr_params *p = (struct thr_params*)arg;
	for (int i=0; i < p->count; ++i)
		write(1,&(p->c),1);
	write(1,"\nBefore cancel\n",sizeof("\nBefore cancel\n"));
	pthread_testcancel();
	write(1,"\nAfter cancel\n",sizeof("\nAfter cancel\n"));
	return NULL;
}
int main() {
	pthread_t thr1_id, thr2_id;
	struct thr_params thr1_params;
	thr1_params.count = 1000;
	thr1_params.c = 'a';
	pthread_create(&thr1_id, NULL, &thr_func, &thr1_params);
	pthread_cancel(thr1_id);
	pthread_join(thr1_id, NULL);
	struct thr_params thr2_params;
	thr2_params.count = 1500;
	thr2_params.c = 'b';
	pthread_create(&thr2_id, NULL, &thr_func, &thr2_params);
	pthread_cancel(thr2_id);
	pthread_join(thr2_id, NULL);
	write(1,"\nin main\n",sizeof("\nin main\n"));
	return 0;
}
