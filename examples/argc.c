#include <stdio.h>
int main(int argc, char *argv[]) {
	printf("Program name: %s\n", argv[0]);
	printf("Parameters count: %d\n", argc-1);
	if (argc > 1)
		for (int i = 1; i < argc; ++i)
			printf("Param # %d: %s\n",i,argv[i]);
	return 0;
}
