#include "functions.h"

void handle_error(char *mess) {
    perror(mess);
    exit(EXIT_FAILURE);
}

int cfg_reader() {
    char port[256]; int flag_port=0;
    char path[256]; int flag_path=0;
    char line[256];
    char str[100];
    char standart[]="# coments\nPORT 8080\nPATH folders/";
    int linenum=0;
    
    FILE * fp;
    if((fp=fopen("server/config.txt", "r+"))==NULL) {   
        printf ("Cannot open file,create new file \n");
        fp=fopen("server/config.txt", "a+t");
        fputs(standart, fp); 
        fclose(fp);
        fp=fopen("server/config.txt", "r+");
    }

    while(fgets(line, 256, fp) != NULL) {
        char buff1[256], buff2[256];

        linenum++;
        if(line[0] == '#') continue;

        if(sscanf(line, "%s %s", buff1, buff2) != 2) {
            fprintf(stderr, "Syntax error, line %d\n", linenum);
            continue;
        }

        if (strcmp(buff1,"PORT")==0) {   
            flag_port=1;
            strcpy(port,buff2);
        }
        if (strcmp(buff1,"PATH")==0) {   
            flag_path=1;
            strcpy(path,buff2);
        }
    }
    fclose(fp);
    if (flag_port==0|| flag_path==0) {
        printf("BAD config file,create new file,old file will be renamed ");
        long int ttime = time (NULL);
        strcpy(str,ctime (&ttime));

        for(int i = 0; str[i] != '\0'; i++) {
            if(str[i]== ' '|| str[i]== '\n') {
                str[i]='_';
            }
        }
        strcat(str,"config.txt");
        printf("Получили: %s\n",str);
        if (-1 == rename ("config.txt",str))
        printf ("Ошибка переименования файла, удалите configs.txt \n");
        else {
            printf ("Выполнено переименование\n");
            cfg_reader(); return -1;
        }  
        fp=fopen("server/config.txt", "a+t");
        fputs(standart, fp);
        fclose(fp);
        
        
    }
    strcpy(PORT,port); // переписываю в глобальные переменные
    strcpy(path_index,path);
    
    printf("\ncfg read:\n\tport-%s\n\tpath-%s\n",port,path);
}