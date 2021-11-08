#include <stdio.h>
#include <unistd.h>
int main() {
	fork();
	fork();
	fork();
	printf("3 times fork\n");
	return 0;
}

