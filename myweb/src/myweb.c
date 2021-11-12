#include <fcntl.h>
#include <pthread.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>
//#include <sys/types.h>
//#include <sys/mman.h>

#include "strfunc.h"


#define HTTP_HEADER_LEN 256
#define HTTP_REQUEST_LEN 256
#define HTTP_METHOD_LEN 6
#define HTTP_URI_LEN 100
#define HTTP_URI_PATH_LEN 40
#define HTTP_URI_PARAM_LEN 40

#define FILE_NAME_LEN 1000
#define LOG_ENTRY_LEN 1000


#define REQ_END 100
#define ERR_NO_URI -100
#define ERR_ENDLESS_URI -101

static char const *index_file = "index.html";


struct http_req {
    char request[HTTP_REQUEST_LEN];
    char method[HTTP_METHOD_LEN];
    char uri[HTTP_URI_LEN];
    char uri_path[HTTP_URI_PATH_LEN];
    char uri_params[HTTP_URI_PARAM_LEN];
    // version
    // user_agent
    // server
    // accept
};

struct thread_params
{
    int argc;
    char** argv;
};

int check_GET_str(char *buf, struct http_req *req)
{
    char *p, *a, *b;
    char* p_up = 0; //Путь (uri path)

    //test
    //    FILE *f = fopen("/var/log/myweb_log/weblog", "a");
    //    fprintf(f, "%s\n", "in check_GET_str");
    //    fclose(f);

    // Это строка GET-запроса
    p = strstr(buf, "GET");
    if (p == buf)
    {
        // Строка запроса должна быть вида
        // GET /dir/ HTTP/1.0
        // GET /dir HTTP/1.1
        // GET /test123?r=123 HTTP/1.1
        // и т.п.
        strncpy(req->request, buf, strlen(buf));
        strncpy(req->method, "GET", strlen("GET"));
        a = strchr(buf, '/');
        if ( a != NULL)
        { // есть запрашиваемый URI
            b = strchr(a, ' ');
            if ( b != NULL )
            { // конец URI
                strncpy(req->uri, a, (unsigned long)(b - a));
                p_up = strchr(a, '?');
                if( p_up != NULL )
                {
                    strncpy(req->uri_path, a, (unsigned long) (p_up - a));
                    strncpy(req->uri_params, p_up + 1, (unsigned long) (b - p_up - 1));
                }
            }
            else
            {
                return ERR_ENDLESS_URI;
                // тогда это что-то не то
            }
        }
        else
        {
            return ERR_NO_URI;
            // тогда это что-то не то
        }
        return 0;
    }
    return -1;
}

int check_Host_str(char *buf, struct http_req *req)
{
    char * p = strstr(buf, "Host");
    if (p == buf)
    {
        //test
        //        FILE *f = fopen("/var/log/myweb_log/weblog", "a");
        //        fprintf(f, "%s\n", "in check_Host_str");
        //        fclose(f);

        strncpy(req->request, buf, strlen(buf));
        return 0;
    }
    return -1;
}

int check_Date_str(char *buf, struct http_req *req)
{
    char * p = strstr(buf, "Date");
    if (p == buf)
    {
        strncpy(req->request, buf, strlen(buf));
        return 0;
    }
    return -1;
}

int fill_req(char *buf, struct http_req *req) {
    if (strlen(buf) == 2) {
        // пустая строка (\r\n) означает конец запроса
        return REQ_END;
    }

    if(check_GET_str(buf, req) == 0)
        return 0;

    //    if(check_Host_str(buf, req) == 0)
    //        return 0;

    //    if(check_Date_str(buf, req) == 0)
    //        return 0;

    return 0;
}

//Непосредственно пишет логи
int write_logs(int handle, const char* str1, const char* str2, bool is_need_to_enter)
{
    if(str1 && str2 && strlen(str1) && strlen(str2))
    {
        char* str = concat_str(str1, str2);
        if (str && write(handle, str, strlen(str)) != (long)strlen(str))
            return 1;

        free(str);

        if(is_need_to_enter)
            write(handle, "\n", 1);
    }
    return 0;
}

int log_req(char* log_path, struct http_req *req)
{
    int fd;

    if ((fd = open(log_path, O_WRONLY | O_CREAT | O_APPEND, 0600)) < 0) {
        perror(log_path);
        return 1;
    }

    struct tm *u;
    const time_t timer = time(NULL);
    u = localtime(&timer);
    char *f = set_time(u);

    if(write_logs(fd, "Date: ", f, true) == 1)
    {
        perror(log_path);
        return 1;
    }

    if(write_logs(fd, "Request: ", req->request, false) == 1)
    {
        perror(log_path);
        return 1;
    }
    if(write_logs(fd, "Method: ", req->method, true) == 1)
    {
        perror(log_path);
        return 1;
    }
    if(write_logs(fd, "Uri path: ", req->uri_path, true) == 1)
    {
        perror(log_path);
        return 1;
    }
    if(write_logs(fd, "Uri params: ", req->uri_params, true) == 1)
    {
        perror(log_path);
        return 1;
    }

    write(fd, "\n", 1);
    fsync(fd);
    close(fd);

    return 0;
}

int make_resp(char *base_path, struct http_req *req)
{
    (void) req;
    //    printf("HTTP/1.1 200 OK\r\n");
    //    printf("Content-Type: text/html\r\n");
    //    printf("\r\n");
    //    printf("<html><body><title>Page title</title><h1>Page HeaderTitle2</h1></body></html>\r\n");

    //    printf("<p>Request:  %s</p>", req->request);
    //    printf("<p>Method: %s</p>", req->method);
    //    printf("<p>Uri: %s</p>", req->uri);
    //    printf("<p>Uri_path: %s</p>", req->uri_path);
    //    printf("<p>Uri_params: %s</p>", req->uri_params);

    int fdin;
    struct stat statbuf;
    void *mmf_ptr;
    // определяем на основе запроса, что за файл открыть
    char res_file[FILE_NAME_LEN] = "";
    if (base_path != NULL) {
        strncpy(res_file,base_path,strlen(base_path));
    }
    strcat(res_file, index_file); // вот сюда писать отображение запроса в файловые пути
    // открываем
    if ((fdin=open(res_file, O_RDONLY)) < 0) {
        perror(res_file);
        return 1;
    }
    // размер
    if (fstat(fdin, &statbuf) < 0) {
        perror(res_file);
        return 1;
    }
    // mmf
    if ((mmf_ptr = mmap(0, statbuf.st_size, PROT_READ, MAP_SHARED, fdin, 0)) == MAP_FAILED)
    {
        perror("myfile");
        return 1;
    }
    // Выводим HTTP-заголовки
    char *http_result = "HTTP/1.1 200 OK\r\n";
    write(1,http_result,strlen(http_result));
    char *http_contype = "Content-Type: text/html\r\n";
    write(1,http_contype,strlen(http_contype));
    char *header_end = "\r\n";
    write(1,header_end,strlen(header_end));
    // Выводим запрошенный ресурс
    if (write(1,mmf_ptr,statbuf.st_size) != statbuf.st_size)
    {
        perror("stdout");
        return 1;
    }
    // Подчищаем ресурсы
    close(fdin);
    munmap(mmf_ptr,statbuf.st_size);

    return 0;
}

void* testThreadFunc(void* arg)
{
    (void) arg;

    int fd;
    const char* log_path = "/var/log/myweb/access.log";

    if ((fd = open(log_path, O_WRONLY | O_CREAT | O_APPEND, 0600)) < 0)
    {
        perror(log_path);
        return (void*) 1;
    }

    for(int i = 0; i < 3; ++i)
    {
        if(write_logs(fd, "MESSAGE: ", "IN_THREAD", true) == 1)
        {
            perror(log_path);
            return (void*) 1;
        }
    }
    return (void*) 0;
}

void* request_received(void* args)
{
    struct thread_params* params = (struct thread_params*) args;
    // первый параметр - каталог с контентом
    // второй параметр - каталог для ведения журнала
    char base_path[FILE_NAME_LEN] = "";
    char log_path[FILE_NAME_LEN] = "";
    char const *log_file = "access.log";

    char buf[HTTP_HEADER_LEN];
    struct http_req req = {0, 0, 0, 0, 0};

//    pthread_t thread;
//    pthread_create(&thread, NULL, &testThreadFunc, NULL);

    // задан каталог журнализации
    if ( params->argc > 2 )
    {
        strncpy(base_path, params->argv[1], strlen(params->argv[1]));
        strncpy(log_path, params->argv[2], strlen(params->argv[2]));
        strcat(log_path, "/");
        strcat(base_path, "/");

        //test version
        //        if ( argc > 3 )
        //        {
        //            while(1)
        //            {
        //                int ret = fill_req(argv[3], &req);
        //                if (ret == 0)
        //                    // строка запроса обработана, переходим к следующей
        //                    continue;
        //                if (ret == REQ_END )
        //                    // конец HTTP запроса, вываливаемся на обработку
        //                    break;
        //                else
        //                    // какая-то ошибка
        //                    printf("Error: %d\n", ret);
        //            }
        //        }
    }
    else
    {
        //test version
        //int ret = fill_req(argv[1], &req);
    }
    strcat(log_path, log_file);

    while(fgets(buf, sizeof(buf),stdin))
    {
        int ret = fill_req(buf, &req);
        if (ret == 0)
            // строка запроса обработана, переходим к следующей
            continue;
        if (ret == REQ_END )
            // конец HTTP запроса, вываливаемся на обработку
            break;
        else
            // какая-то ошибка
            printf("Error: %d\n", ret);
    }

    log_req(log_path, &req);
    make_resp(base_path, &req);

    //pthread_join(thread, NULL);

    return (void*) 0;
}


int main (int argc, char* argv[])
{
    struct thread_params params;
    params.argc = argc;
    params.argv = argv;

    pthread_t thread;
    pthread_create(&thread, NULL, &request_received, &params);
    pthread_join(thread, NULL);

    return 0;
}
