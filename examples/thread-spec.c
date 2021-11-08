#include <malloc.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

pthread_key_t thread_key;
FILE* logfile;

void write_to_log() {
	char* message =  (char *)pthread_getspecific(thread_key);
	fprintf(logfile, "%s\n", message);  
}
void* thread_func(void *args) {
	char thread_var[30];
	sprintf(thread_var, "Thread ID: %lu\n",pthread_self());
	pthread_setspecific(thread_key, thread_var);
	write_to_log();
	return NULL;
}
int main() {
	char *log_file_name = "threads.log"; 
	logfile = fopen(log_file_name, "w");
	pthread_t threads[5];
	pthread_key_create(&thread_key, NULL);
	int i;
	for (i=0; i < 5; i++) 
		pthread_create(&(threads[i]), NULL, thread_func, NULL);
	for (i=0; i < 5; i++) 
		pthread_join(threads[i], NULL);
	fclose(logfile);
	return 0;
}

