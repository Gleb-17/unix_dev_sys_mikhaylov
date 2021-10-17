#include <stdio.h>
#include <string.h>


#include "journal.h"

#define HTTP_HEADER_LEN 256
#define HTTP_REQUEST_LEN 256
#define HTTP_METHOD_LEN 6
#define HTTP_URI_LEN 100
#define HTTP_URI_PATH_LEN 40
#define HTTP_URI_PARAM_LEN 40

#define REQ_END 100
#define ERR_NO_URI -100
#define ERR_ENDLESS_URI -101



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

int check_GET_str(char *buf, struct http_req *req)
{
    char *p, *a, *b;
    char* p_up = 0; //Путь (uri path)

    //test
    FILE *f = fopen("/var/log/myweb_log/weblog", "a");
    fprintf(f, "%s\n", buf);
    fclose(f);

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
                strncpy(req->uri, a, b - a);
                p_up = strchr(a, '?');
                if( p_up != NULL )
                {
                    strncpy(req->uri_path, a, p_up - a);
                }
                strncpy(req->uri_params, p_up, b - p_up);
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

    if(check_Host_str(buf, req) == 0)
        return 0;

    if(check_Date_str(buf, req) == 0)
        return 0;

    return 0;
}

int log_req(struct http_req *req)
{
    if(check_directory())
        return -1;

    FILE* f = NULL;
    check_file(&f);

    if(f == NULL)
        return -1;

    fprintf(f, "%s\n", req->request);
    fprintf(f, "%s\n", req->method);
    fprintf(f, "%s\n", req->uri);

    //fprintf(stderr, "%s %s\n%s\n", req->request, req->method, req->uri);

    fclose(f);
	return 0;
}

int make_resp(struct http_req *req) {
	printf("HTTP/1.1 200 OK\r\n");
	printf("Content-Type: text/html\r\n");
	printf("\r\n");
	printf("<html><body><title>Page title</title><h1>Page HeaderTitle2</h1></body></html>\r\n");

	printf("<p>Request:  %s</p>", req->request);
	printf("<p>Method: %s</p>", req->method);
	printf("<p>Uri: %s</p>", req->uri);
	printf("<p>Uri_path: %s</p>", req->uri_path);
	printf("<p>Uri_params: %s</p>", req->uri_params);

	return 0;
}

int main ()
//int main (int argc, char* argv[])
{
	char buf[HTTP_HEADER_LEN];
    struct http_req req = {0};

//    if(argc > 1)
//    {
//        int ret = fill_req(argv[1], &req);
//        if (ret != 0)
//            // строка запроса обработана, переходим к следующей
//            return -1;
//        if (ret == REQ_END )
//            // конец HTTP запроса, вываливаемся на обработку
//            return 0;
//        else
//            // какая-то ошибка
//            printf("Error: %d\n", ret);

//    }
//    else
//    {
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
//    }

	log_req(&req);
	make_resp(&req);
}
