#include <stdlib.h>
#include <stdio.h>
int main() {
	char *server = getenv("SERVER");
	if (server != NULL) 
		printf("Getting access to: %s\n",server);
	else
		printf("Server not set\n");
	return 0;
}
