#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <time.h>

int shm_id=0;

int gen_random(int *share) {
	int num;
	srand(time(NULL));
	for (int i=0; i < 30; i++) {
		num = random() % 1000;
		*share = num;
		printf("-PARENT- Value: %d\n", num);
		sleep(1);
	}
	return 0;
}

int get_random(int *share) {
        for (int i=0; i < 30; i++){
               	sleep(1);
               	printf("-CHILD - Value: %d\n", *share);
        }
	return 0;
}

int get_shm() {
	int id;
	key_t key;
	if ((key = ftok("shmfork", 1)) == -1 ) {
		perror("ftok()");
		exit(1);
	}
	if ((id = shmget (key, getpagesize(), IPC_CREAT|IPC_EXCL|S_IRUSR|S_IWUSR)) == -1 ) {
		perror("shmget()");
		exit(1);
	}
	return id; 
}

int main () {
	int *share, *child_ptr;
	pid_t child_pid;
	shm_id = get_shm(); // выделяем SHM, ее ID будет у обоих процессов
        child_pid = fork();
        if (child_pid != 0) { // в родителе
		if ((share = shmat(shm_id, NULL, SHM_RND)) == (void *)-1 ) {
			perror("shmat()");
			exit(1);
		} // подключили разделяемую память
		printf("-PARENT- Got SHMEM segment %d attached at: %p\n",shm_id, share);
		gen_random(share); // пишем случайные числа
		printf("-PARENT- wait for child\n");
		wait(NULL); // ждем потомка
		shmdt(share);
		shmctl(shm_id, IPC_RMID,0); // освобождаем ресурсы 
		printf("-PARENT- FINISH\n");
	}
        else  { // в потомке
	        if ((share = shmat(shm_id, NULL, SHM_RND|SHM_RDONLY)) == (void *)-1 ) {
                	perror("shmat()");
                	exit(1);
        	} // подключили разделяемую память на чтение
        	printf("-СHILD - Got SHMEM segment %d attached at: %p\n",shm_id, share);
        	get_random(share);
		shmdt(share); // отцепили память
		printf("-CHILD - FINISH\n");
	}
	return 0;
}

