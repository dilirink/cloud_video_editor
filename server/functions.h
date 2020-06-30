#ifndef _CLOUD_VIDEO_EDITOR_SERVER_
#define _CLOUD_VIDEO_EDITOR_SERVER_

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <sys/wait.h>
#include <signal.h>
#include <time.h>

#define RED   "\x1B[31m"
#define GRN   "\x1B[32m"
#define YEL   "\x1B[33m"
#define BLU   "\x1B[34m"
#define MAG   "\x1B[35m"
#define CYN   "\x1B[36m"
#define WHT   "\x1B[37m"
#define RESET "\x1B[0m" // example  printf("This is " RED "red" RESET " and this is " BLU "blue" RESET "\n");

#define LEN 10000
#define MAX_CONNECTION 1000
char PORT[10];
char path_index[100];
int size_file_open_and_read;

typedef struct {
    char *ext;
    char *mediatype;
    int number;
} extn;

void handle_error(char *);
//void init_extn();
int cfg_reader();
char *file_open_and_read(char *);
void *get_in_addr(struct sockaddr *);
int http_request_type(char *); 
int get_searcher(char *, int, extn *);

#endif