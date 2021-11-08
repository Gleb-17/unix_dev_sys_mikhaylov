#include <malloc.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

pthread_key_t log_file_key;

void write_to_log(const char *message) {
	FILE* log =  (FILE*)pthread_getspecific(log_file_key);
	fprintf(log, "%s\n", message);  
}
void close_log(void* log) {
	fclose((FILE*)log);
}
void* thread_func(void *args) {
	char log_file_name[30];
	sprintf(log_file_name, "thread-%lu.log",pthread_self());
	FILE* log = fopen(log_file_name, "w");
	pthread_setspecific(log_file_key, log);
	write_to_log("Thread start\n");
	return NULL;
}
int main() {
	pthread_t threads[5];
	pthread_key_create(&log_file_key, close_log);
	int i;
	for (i=0; i < 5; ++i) 
		pthread_create(&(threads[i]), NULL, thread_func, NULL);
	for (i=0; i < 5; ++i) 
		pthread_join(threads[i], NULL);
	return 0;
}

