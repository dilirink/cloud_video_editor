#include "functions.h"

//   ./server > log_server & disown pid

    // схема создания сокета для сервера
    //getaddrinfo()
    // socket()
    // bind()
    // listen()
    // accept()

//поисково ответвная структура.

/**/

//новинка
//int post_parser(char *massage);
//char* post_seve_file(char *s,int fd_new);
//новинка_ценок
//void print(char *strok, int param);

int main(int argc, char *argv[]) { //для передачи или порта или управляюших ключей или колличества пользователей НА БУДУЩИЕ:)   
    extn extensions[] = {
        {".gif", "Content-Type: image/gif\r\n\r\n" ,1},
        {".txt", "Content-Type: text/plain\r\n\r\n",2},
        {".jpg", "Content-Type: image/jpg\r\n\r\n" ,3},
        {".jpeg","Content-Type: image/jpeg\r\n\r\n",4},
        {".png", "Content-Type: image/png\r\n\r\n" ,5},
        {".ico", "Content-Type: image/ico\r\n\r\n" ,6},
        {".zip", "Content-Type: image/zip\r\n\r\n" ,7},
        {".gz",  "Content-Type: image/gz\r\n\r\n"  ,8},
        {".tar", "Content-Type: image/tar\r\n\r\n" ,9},
        {".htm", "Content-Type: text/html\r\n\r\n" ,10},
        {".html","Content-Type: text/html\r\n\r\n" ,11},
        {"GET / HTTP/1.1","Content-Type: text/html\r\n\r\n",12},
        {".php","Content-Type: text/html\r\n\r\n" ,13},
        {".pdf","Content-Type: application/pdf\r\n\r\n",14},
        {".zip","Content-Type: application/octet-stream\r\n\r\n",15},
        {".rar","Content-Type: application/octet-stream\r\n\r\n",16},
        {".css","Content-Type: text/css\r\n\r\n"},
        {".js","Content-type: application/javascript\r\n\r\n"},
        {".mp4", "Content-Type: video/mp4\r\n\r\n" ,1},
        {0,0} 
    };
    
    int http_get_errors = 0; //для обработки ошибок
    int http_index_html = 0;//для проверки запроса Index.html  
    int i;
    int sock, yes = 1,rv;;
    int sockfd, new_fd;  // слушаем 
    const int request_buffer_size = 65536;
    char str2[] = "www/index.html";
    char *read_file_buffer;
    char s[INET6_ADDRSTRLEN];
    //char      request[request_buffer_size];
    char *request = (char *) malloc(15000000);
    if(request == NULL) { printf("sistema zajala pameti"); exit (1); }
    struct addrinfo hints, *servinfo, *p;
    struct sockaddr_storage their_addr; // адресная информация
    socklen_t sin_size;
    
    ////отправляемые данные

    if (cfg_reader()==-1)
        printf(RED"eror cfg_reader\n"RESET); //заполняем путь и порт из файла конфинурации!
    
    printf(CYN"\n\n\nServer: Web Server in C start!\r\n\n\n"RESET);
   
    memset(&hints, 0, sizeof(struct addrinfo)); //занулили
    hints.ai_family = AF_INET;    /* Allow IPv4 or IPv6 */
    hints.ai_socktype = SOCK_STREAM; /* Datagram socket */
    hints.ai_flags = AI_PASSIVE;    /* For wildcard IP address */
    hints.ai_protocol = 0;          /* Any protocol */

    if ((rv = getaddrinfo(NULL, PORT, &hints, &servinfo)) != 0) {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
        return 1;
    }

    for(p = servinfo; p; p = p->ai_next) {
        if((sockfd = socket(p->ai_family,p->ai_socktype ,p->ai_protocol)) == -1)
            handle_error("server:socket");

        if(setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) == -1)
            handle_error("setsockop");

        if(bind(sockfd, p->ai_addr, p->ai_addrlen) == -1) {
            close(sockfd);
            perror("server: bind");
            continue;
        }
        break;
    }
    
    freeaddrinfo(servinfo);
    
    if(p == NULL)
	    handle_error("server: failed to bind");

    if(listen(sockfd, MAX_CONNECTION) == -1) 
	    handle_error("listen");
         
    int bytes_recvd = 0, breaks;
    char key[50], filename[50], *razdel;
    int file_size = 0;
    int key_my = 0,otvet_nomer = 0;

    char *ach, *file, *len, len_buff[20];
    
    while(1) {
        sin_size = sizeof(their_addr);
        new_fd = accept(sockfd, (struct sockaddr *)&their_addr, &sin_size);
        if(new_fd == -1) {
            perror("accept");
            continue;
        }
        inet_ntop(their_addr.ss_family,
        get_in_addr((struct sockaddr *)&their_addr),
        s, sizeof s);
        printf(RESET"server: got connection from %s\n", s);

        //////////////////////////////////////////////////////////////////////////////////////выдача get запроса в терминал
        
        bytes_recvd = recv(new_fd, request, request_buffer_size - 1, 0);
        if (bytes_recvd < 0)
          fprintf(stderr, "error recv\n");
        
        printf("\n\n\n"RED"recv:"GRN"%s\n\n\n"RESET, request);
        http_get_errors = http_request_type(request); // проверка GET ли это или нет

        if(http_get_errors == 1) {
            printf(RESET"\naga POST zapros prishOOOL,bytes_recvd=%d\n\n",bytes_recvd);
            if(!fork()) { 
                while(1) {  
                    char otvet[256]="HTTP/1.1 200 OK\r\nServer: Web Server in C\r\n";

                    i = 0;
                    breaks = 0;
                    key_my = 0;
                    otvet_nomer = 0;
                    
                  /*  char *resume="HTTP/1.1 308 Resume Incomplete,\r\n";
                    char *ok="HTTP/1.1 200 OK\r\n";  //r\n\r\n
                    char *server="Server: Web Server in C\r\n";
                    char *location="Location:http://dilirink.dlinkddns.com:3491/{";   */
                    char *razdel = strtok(request, "\n"); // http://all-ht.ru/inf/prog/c/func/strtok.html немного гайда 
                    while(razdel != NULL) {
                        //printf ("%d)%s\n",i,razdel);
                        i++;
                        
                        ach = strstr(razdel,"boundary=");
                        file = strstr(razdel,"filename");
                        len = strstr(razdel,"Content-Length:");
                        if(ach != NULL && key_my != 1) {   
                            for(int y = ach - razdel + 8, k = 0; razdel[y] != '\0'; y++, k++)
                                key[k] = razdel[y];    
                            printf("key=\n|%s|\n", key);
                            
                        }
                        if(file != NULL) {
                            for(int a = file - razdel + 10, s = 0; razdel[a] != '\"'; a++, s++)
                            filename[s] = razdel[a];
                            printf("filename=|%s|\n",filename);
                        }
                        if(len != NULL) {
                            for(int l = len - razdel + 16, lol = 0; razdel[l] != '\0'; l++, lol++)
                            len_buff[lol] = razdel[l];
                            file_size = atoi(len_buff);
                            printf("size file=%d\n", file_size);
                        }
                        razdel = strtok(NULL, "\n");
                    }   
                    if(send(new_fd, otvet, strlen(otvet), 0) == -1)
                        perror("EROR in send in get_searcher");
                    int bytes_recvd = recv(new_fd, request, request_buffer_size - 1, 0);
                }
                close(new_fd);
                exit(0); 
            }   
            close(new_fd);
        }
        if(http_get_errors == 2) {   ////// GET
            printf(RESET"\naga GET zapros prishOOOL,bytes_recvd=%d\n\n", bytes_recvd);
            if(!fork()) { // this is the child process
                close(sockfd); //у нас есть уже от ассепта этот для отдельного процесса не нужен

                char *razdel = strtok(request, "\n"); // http://all-ht.ru/inf/prog/c/func/strtok.html немного гайда
                int i = 0, breaks = 0;
                while(razdel != NULL) {
                    // Вывод очередной выделенной части
                    printf("%d)%s\n", i, razdel);
                    i++;
                    // Выделение очередной части строки
                    breaks = get_searcher(razdel, new_fd, extensions);
                    if(breaks = 1) break;
                    else razdel = strtok(NULL, "\n");
                }   
                close(new_fd);
                exit(0);
            }
            close(new_fd);  // parent doesn't need this
        }
        if(http_get_errors == 3) {  /////// i ne GET i ne POST
            printf(YEL"\n\n\n\nprishlo chto to strannoe NOT GET NOT POST: "RESET);
            printf(RED"%s\n\n\n"RESET, request);
        }       
    }
    free(request);
    return 0;
}

//тут я просто сохранил гет запрос для себя)
// server: got connection from 192.ххх.0.ххх
// request:
// GET / HTTP/1.1
// Host: dilirink.dlinkddns.com:3491
// Connection: keep-alive
// Cache-Control: max-age=0
// Upgrade-Insecure-Requests: 1
// User-Agent: Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/80.0.3987.163 Safari/537.36
// Accept: text/html,application/xhtml+xml,application/xml;q=0.9,image/webp,image/apng,*/*;q=0.8,application/signed-exchange;v=b3;q=0.9
// Accept-Encoding: gzip, deflate
// Accept-Language: ru-RU,ru;q=0.9,en-US;q=0.8,en;q=0.7


// server: got connection from 192.168.0.87
// request:
// GET /favicon.ico HTTP/1.1
// Host: dilirink.dlinkddns.com:3491
// Connection: keep-alive
// Pragma: no-cache
// Cache-Control: no-cache
// User-Agent: Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/80.0.3987.163 Safari/537.36
// Accept: image/webp,image/apng,image/*,*/*;q=0.8
// Referer: http://dilirink.dlinkddns.com:3491/
// Accept-Encoding: gzip, deflate
// Accept-Language: ru-RU,ru;q=0.9,en-US;q=0.8,en;q=0.7



// recv:POST / HTTP/1.1
// Host: dilirink.dlinkddns.com:3491
// User-Agent: Mozilla/5.0 (Windows NT 10.0; Win64; x64; rv:77.0) Gecko/20100101 Firefox/77.0
// Accept: */*
// Accept-Language: ru-RU,ru;q=0.8,en-US;q=0.5,en;q=0.3
// Accept-Encoding: gzip, deflate
// X-Requested-With: XMLHttpRequest
// Content-Type: multipart/form-data; boundary=---------------------------340475080942671201852026098931
// Content-Length: 11069542
// Origin: http://dilirink.dlinkddns.com:3491
// Connection: keep-alive
// Referer: http://dilirink.dlinkddns.com:3491/

// -----------------------------340475080942671201852026098931
// Content-Disposition: form-data; name="file"; filename="videoplayback.3g2"
// Content-Type: video/3gpp2