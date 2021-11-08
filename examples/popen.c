#include <stdio.h>
#include <unistd.h>

const char *string = "test data";

int main() {
	FILE *stream = popen("tr [a-z] [A-Z]", "w");
	fprintf(stream, "%s\n", string);
	return pclose(stream);
}
