#include <unistd.h>
#include <stdio.h>
int main() {
	printf("String\n");
	fork();
	return 0;
}

