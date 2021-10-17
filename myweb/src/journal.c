#include "journal.h"

#include <dirent.h>
#include <errno.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>

static const char* logfileDir = "/var/log/myweb_log";
static const char* logfile = "/var/log/myweb_log/weblog";
static DIR* dir = NULL;


int print_error()
{
    printf("%s\n", strerror(errno));
    return -1;
}

int check_directory()
{
    if ((dir = opendir(logfileDir)) == NULL)
    {
        if (mkdir(logfileDir, 0755) < 0)
            return print_error();
        else
        {
            int res = chdir(logfileDir);
            if(res == -1)
                return print_error();
        }
    }
    return 0;
}

void check_file(FILE** f)
{
    if ((*f = fopen(logfile, "r")) == NULL)
    {
        //файл отсутствует
        if ((*f = fopen(logfile, "w")) == NULL)
            print_error();

        if (chmod(logfile, S_IRWXU | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH ) < 0)
            print_error();
    }
    else
    {
        //файл уже существует
        fclose(*f);
        *f = fopen(logfile, "a");
    }
}
