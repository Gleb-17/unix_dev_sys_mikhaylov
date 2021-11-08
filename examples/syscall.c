#include <unistd.h>
#include <sys/syscall.h>
int main() {
	char msg[9] = "Syscall!\n";
	syscall(__NR_write, 1, msg, 9);
	return 0;
}
