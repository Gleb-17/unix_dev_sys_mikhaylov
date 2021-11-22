#include <pthread.h>
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <semaphore.h>
#include <unistd.h>

struct job {  // храним задания как связанный список
	struct job *next;
};

struct job *job_queue;

pthread_mutex_t mtx = PTHREAD_MUTEX_INITIALIZER; // мьютекс, защищающий очередь

sem_t job_queue_count; // семафор, подсчитывающий задания в очереди

int pr_exit(char const* str) {
        perror(str);
        exit(errno);
}

void init_job_queue() {
	job_queue = NULL;
	sem_init(&job_queue_count, 0, 0);
}


void process_job(struct job *run_job) { 
	printf("Thread %lu finished job\n", pthread_self());
}

void* thr_func(void* arg) {
	int loops = *((int *) arg);
	int local;
	for (int i = 0; i < loops; i++) {
		struct job *next_job;
		sem_wait(&job_queue_count);
		if (pthread_mutex_lock(&mtx) != 0) pr_exit("pthread_mutex_lock");
		next_job = job_queue;
		job_queue = job_queue->next; // извлекаем задание из очереди
		if (pthread_mutex_unlock(&mtx)!=0) pr_exit("pthread_mutex_unlock");
		process_job(next_job);
		free(next_job);
	}
	return NULL;
}

void enqueue_job() {
	struct job *new_job;
	new_job = (struct job*)malloc(sizeof(struct job));
	if (pthread_mutex_lock(&mtx) != 0) pr_exit("pthread_mutex_lock");
	new_job->next = job_queue;
	job_queue = new_job; // помещаем задание в начало очереди
	sem_post(&job_queue_count); // устанавливаем семафор - есть задание в очереди
	if (pthread_mutex_unlock(&mtx)!=0) pr_exit("pthread_mutex_unlock");
}

int main(int argc, char *argv[]) {
	pthread_t thr_id1, thr_id2;
	int loops;
	if (argc > 1)
		loops = atoi(argv[1]); 
	else
		loops = 100;
	printf("Loops: %d\n", loops);
	init_job_queue();
	if (pthread_create(&thr_id1, NULL, &thr_func, &loops) !=0) pr_exit("pthread_create");
	if (pthread_create(&thr_id2, NULL, &thr_func, &loops) !=0) pr_exit("pthread_create");
	for (int i=0; i < loops; i++) {
		enqueue_job();
		sleep(1);
	}
	printf("Jobs finished\n");
	if (pthread_join(thr_id1, NULL) !=0) pr_exit("pthrread_join");
	if (pthread_join(thr_id2, NULL) !=0) pr_exit("pthread_join");
	return 0;
}
