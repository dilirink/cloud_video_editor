#include "functions.h"

int main(int argc, char *argv[]) {
    int sockfd, rv, yes = 1;
    struct addrinfo hints, *servinfo, *p;
    struct sockaddr_storage their_addr;
    socklen_t sin_size;

    if(cfg_reader() == -1)
        printf(RED"eror cfg_reader\n"RESET); 

    printf(CYN"\n\n\nServer: Web Server in C start!\r\n\n\n"RESET);
   
    memset(&hints, 0, sizeof(struct addrinfo));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE;
    hints.ai_protocol = 0;

    if ((rv = getaddrinfo(NULL, PORT, &hints, &servinfo)) != 0) {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
        return 1;
    }

    for(p=servinfo; p!=NULL; p=p->ai_next){
       if((sockfd = socket(p->ai_family,p->ai_socktype ,p->ai_protocol))==-1) {
           perror("server:socket");
           exit(1);
       }

       if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int))==-1) {
            perror("setsockopt");
            exit(1);
       }

        if (bind(sockfd, p->ai_addr, p->ai_addrlen) == -1) {
            close(sockfd);
            perror("server: bind");
            continue;
        }

        break;
    }
    
    freeaddrinfo(servinfo);
    
    if (p == NULL) {
	    fprintf(stderr, "server: failed to bind\n");
	    exit(1);
    }

    if (listen(sockfd, MAX_CONNECTION) == -1) {
	    perror("listen");
	    exit(1);
    }

    ////////////////HERE SHOULD BE MAIN BODY/////////////////////////////

    shutdown(sockfd, SHUT_RDWR);
    return 0;
}