#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>

const char *logfile = "access.log";

int write_to_journal(char *entry) {
	int fd = open(logfile, O_WRONLY | O_CREAT | O_APPEND, 0600);
	write(fd, entry, strlen(entry));
	write(fd, "\n", 1);
	fsync(fd);
	close(fd);
	return 0;
}
int main() {
	if (access(logfile,F_OK|W_OK) == 0 )
		write(1,"File exists\n",12);
	else 
		write(1, "No file\n",8);
	return write_to_journal("test journal message");
}
