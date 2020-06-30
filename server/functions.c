#include "functions.h"

void handle_error(char *mess) {
    perror(mess);
    exit(EXIT_FAILURE);
}

/*void init_extn() {
    strcpy(extensions[0].ext, ".gif");
    strcpy(extensions[0].mediatype, "Content-Type: image/gif\r\n\r\n");
    extensions[0].number = 1;

    strcpy(extensions[0].ext, ".gif");
    strcpy(extensions[0].mediatype, "Content-Type: image/gif\r\n\r\n");
    extensions[0].number = 1;

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
}*/

int cfg_reader() { //тут без бутылки ноги сломаешь, но под чай тоже можно понять, читая это через почти месяц написания я почти понял))
    char port[256]; int flag_port = 0;
    char path[256]; int flag_path = 0;
    char line[256];
    char str[100];
    char standart[] = "# coments\nPORT 8080\nPATH folders/";
    int linenum=0;
    
    FILE *fp;
    if((fp = fopen("server/config.txt", "r+")) == NULL) {   
        printf("Cannot open file,create new file \n");
        fp = fopen("server/config.txt", "a+t");
        fputs(standart, fp); 
        fclose(fp);
        fp = fopen("server/config.txt", "r+");
    }
    while(fgets(line, 256, fp) != NULL) {
        char buff1[256], buff2[256];

        linenum++;
        if(line[0] == '#') continue;

        if(sscanf(line, "%s %s", buff1, buff2) != 2)    {
            fprintf(stderr, "Syntax error, line %d\n", linenum);
            continue;
        }

        if(strcmp(buff1, "PORT") == 0) {   
            flag_port = 1;
            strcpy(port, buff2);
        }
        if(strcmp(buff1, "PATH") == 0) {   
            flag_path = 1;
            strcpy(path, buff2);
        }
    }
    fclose(fp);
    if(flag_port == 0 || flag_path == 0) {
        printf("BAD config file,create new file,old file will be renamed ");
        long int ttime = time (NULL);
        strcpy(str, ctime(&ttime));

        for(int i = 0; str[i] != '\0'; i++) {
            if(str[i]== ' '|| str[i]== '\n')
                str[i]='_';
        }

        strcat(str, "server/config.txt");
        printf("Получили: %s\n",str);
        if(-1 == rename("server/config.txt",str))
        printf ("Ошибка переименования файла, удалите configs.txt \n");
        else {
            printf ("Выполнено переименование\n");
            cfg_reader();
            return -1;
        }  
        fp = fopen("server/config.txt", "a+t");
        fputs(standart, fp);
        fclose(fp);
        
        
    }
    strcpy(PORT, port); // переписываю в глобальные переменные
    strcpy(path_index, path);
    
    printf("\ncfg read:\n\tport-%s\n\tpath-%s\n",port, path);
}

void *get_in_addr(struct sockaddr *sa) {
    if (sa->sa_family == AF_INET)
        return &(((struct sockaddr_in*)sa)->sin_addr);
 
    return &(((struct sockaddr_in6*)sa)->sin6_addr);
}

int http_request_type(char *recv_message) {
    int i = 0;
    int stranno = 1; /// крч если не гет или пост то мы вообще хз что это)
    int ret = 0;
    char massage[255] = {0};
    sscanf(&recv_message[i], "%s", massage);

    if (massage[0] == 'P' && massage[1] == 'O' && massage[2] == 'S' && massage[3] == 'T') {
        ret = 1;
        stranno = 0;
    }
    if (massage[0] == 'G' && massage[1] == 'E' && massage[2] == 'T') {
        ret = 2;
        stranno = 0;
    }
    if(stranno == 1)    ret=3;
    return ret;
}

char *file_open_and_read(char *s) { 
    char *eror = "EROR204";
    FILE *ptrFile = fopen(s, "r+b");
 
    if (ptrFile == NULL) { //обраюотка ошибки 204 нет контента 
        printf("Ошибка файла|%s|\n", s);
        return eror;
    }
 
  // определяем размер файла
    fseek(ptrFile , 0 , SEEK_END);                          // устанавливаем позицию в конец файла
    long lSize = ftell(ptrFile);                            // получаем размер в байтах
    rewind(ptrFile);                                       // устанавливаем указатель в конец файла
 
    char *buffer = (char*)malloc(sizeof(char) * lSize); // выделить память для хранения содержимого файла
    if(buffer == NULL) {
        fputs("Ошибка памяти", stderr);
        exit(2);
    }
 
    size_t result = fread(buffer, 1, lSize, ptrFile);       // считываем файл в буфер
    if (result != lSize) {
        fputs("Ошибка чтения", stderr);
        exit (3);
    }
    size_file_open_and_read=result;
    printf("size read file =%d\n", size_file_open_and_read);
    //содержимое файла теперь находится в буфере
    // puts(buffer);
    // завершение работы
    fclose(ptrFile);

 /* отчистить после использование*/
 // free (buffer);
  
    return buffer;
}

int get_searcher(char *s, int fd_new, extn *extensions) { //открываем нужный файл и сразу отправляем)
    char line[256];
    char *read_file_buffer;
    char buffer[256];
    char buffer2[256];
    FILE *mf;
    char str1[LEN] = "HTTP/1.1 200 OK\r\nServer: Web Server in C\r\n",
    str2[] = "www/index.html",
    eror[LEN] = "HTTP/1.1 204 No Content\r\nServer: Web Server in C\r\n";
  
    for (int i = 0; extensions[i].ext != NULL; i++) {
        char *lo = strstr(s, extensions[i].ext);
        if(lo != NULL) {  
            for (int bf = 5,fb = 0; bf < (int)(lo-s); fb++, bf++) {
                buffer[fb] = s[bf]; 
            }

            printf("buffer size= %ld", strlen(buffer));
            int size = strlen(buffer);
            while(1) {  /// чистим хвосты у файлов,там какие то кракозабры приклеивались, читали лишнее и приходилось вычищать. 
                char *istr = strstr(s, buffer);

                //Вывод результата поиска на консоль
                if ( istr == NULL) {
                    buffer[size] = '\0';
                    size--;
                }
                else  
                    break;
            }

            if(extensions[i].number == 12) {   
                strcpy(buffer, path_index);
                strcat(buffer, "index.html");
                read_file_buffer = file_open_and_read(buffer);
                if(send(fd_new, str1, strlen(str1), 0) == -1)
                    perror("EROR in send in get_searcher");
                if(send(fd_new, extensions[i].mediatype , strlen(extensions[i].mediatype), 0) == -1)
                    perror("EROR in send in get_searcher");
                if(send(fd_new, read_file_buffer, size_file_open_and_read, 0) == -1)
                    perror("EROR in send in get_searcher");
                memset(buffer, '\0', 256); 
                return 1;
            }
            else {
                strcpy(buffer2, path_index);
                printf(YEL"file |%s|  size  send any file\n"RESET,buffer);
                strcat(buffer, extensions[i].ext);
                strcat(buffer2, buffer);
                printf(BLU"file |%s|  size  send any file %d\n"RESET, buffer, size_file_open_and_read);
                read_file_buffer = file_open_and_read(buffer2);

                if(strstr(read_file_buffer,"EROR204")!=NULL) { //обрабатываем момент что файла нету в дериктории   
                    printf(RED"file EROR sen 204 No Content\n"RESET);
                    if(send(fd_new, eror, strlen(eror), 0) == -1)
                        perror("EROR in send in get_searcher");
                }
                printf(BLU"file |%s|  size  send any file %d\n"RESET, buffer, size_file_open_and_read);
                if (send(fd_new, str1, strlen(str1), 0) == -1)
                    perror("EROR in send in get_searcher");
                if (send(fd_new, extensions[i].mediatype, strlen(extensions[i].mediatype), 0) == -1)
                    perror("EROR in send in get_searcher");
                if (send(fd_new, read_file_buffer, size_file_open_and_read, 0) == -1)
                    perror("EROR in send in get_searcher");
                memset(buffer, '\0', 256);  
                return 1;
            }     
        }
        
    }
    free(read_file_buffer);
    close(fd_new);
}