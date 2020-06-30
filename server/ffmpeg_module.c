#include "functions.h"

char *ffmpeg(char *filename, char *outputname, int param) {
    pid_t pid;
    int ret,status;
    char fullname_output[256], str_system[256] = {0}, str_for_return[256];
    
    if(!mkdir(outputname, 0777))
        printf("directory |%s| created successfully\n",outputname);
    else
        printf("ERROR creat %s\n",outputname);

    strcpy(fullname_output, "./");    
    strcat(fullname_output, outputname);   
    strcat(fullname_output, "/");
    strcat(fullname_output, outputname);
    printf("EXEC strock: %s\n", fullname_output);

    pid = fork();
    switch(param) {
        case 1: //отрезаем мп3
        if(pid == 0) {
            strcat(fullname_output, ".mp3");
            execlp("ffmpeg", "ffmpeg", "-i", filename, fullname_output, (char*)NULL);   
            exit(10);
        }
        else {
            printf("____ESLE__\n");
            waitpid(pid, &status, 0);
            if(WEXITSTATUS(status) == 0) {
                strcat(fullname_output,".mp3");
                printf("FFMPEG SUCSES %s\n", fullname_output);
                printf("open_file_and_read\n");
                return file_open_and_read(fullname_output);
            }      
            else
                printf("ERROR FFMPEG %s\n",fullname_output);
        } 
        break;

        case 2: /// на скриншоты
        if(pid == 0) {   
            strcat(fullname_output, "_%03d.jpg");
            execlp("ffmpeg", "ffmpeg", "-i", filename, fullname_output, (char*)NULL);
            exit(10);
        }

        else {
            waitpid(pid, &status, 0);
            if(WEXITSTATUS(status)==0){
                char buffer[256];
                strcpy(str_system,"tar cf ");
                strcat(str_system, outputname);  // tar cf asd.tar ./asd/*.jpg      // rm -R asd
                strcat(str_system,".tar ");
                strcat(str_system, "./");
                strcat(str_system,outputname);
                strcat(str_system,"/*.jpg; rm -R ");
                strcat(str_system,outputname);
                printf("system: %s\n",str_system); 
                if(system(str_system)==0)
                    printf("ERROR SYSTEM CALL: %s\n",str_system);
                 
                printf("open_file_and_read\n");
                strcpy(buffer,outputname);
                strcat(buffer,".tar");
                return file_open_and_read(buffer);
            }
            else
                printf("ERROR FFMPEG %s\n", fullname_output);
        }
        break;
 
    }   
}