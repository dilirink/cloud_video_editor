#include<stdio.h>
#include<stdlib.h>
#include <string.h>


int main()
{
char request[256]={"X-Requested-With: XMLHttpRequest\nContent-Type: multipart/form-data; boundary=---------------------------340475080942671201852026098931\nContent-Length: 11069542\nContent-Disposition: form-data; name=\"file\"; filename=\"videoplayback.3g2\""};
//scanf("%s",request);

    char *razdel=strtok(request,"\n"); // http://all-ht.ru/inf/prog/c/func/strtok.html немного гайда
    int i=0,breaks=0;
    char key[50],filename[50];
    int file_size;
    while (razdel != NULL) 
    {
        i++;
        char *ach,*file,*len,len_buff[20];
        ach=strstr(razdel,"boundary=");
        file=strstr(razdel,"filename");
        len=strstr(razdel,"Content-Length:");
        if (ach!=NULL)
        {   
            for(int y=ach-razdel+8,k=0;razdel[y]!='\0';y++,k++)
            key[k]=razdel[y];    
            
            printf("key=%s\n",key);
        }
        if (file!=NULL)
        {
            for(int a=file-razdel+10,s=0;razdel[a]!='\"';a++,s++)
            filename[s]=razdel[a];

            printf("filename-|%s|\n",filename);
        }
        if(len!=NULL)
        {
            for(int l=len-razdel+16,lol=0;razdel[l]!='\0';l++,lol++)
            len_buff[lol]=razdel[l];
            file_size=atoi(len_buff);
            printf("size file=%d\n",file_size);
        }
        razdel = strtok (NULL,"\n");
    }   
}

/*
POST / HTTP/1.1
Host: dilirink.dlinkddns.com:3491
User-Agent: Mozilla/5.0 (Windows NT 10.0; Win64; x64; rv:77.0) Gecko/20100101 Firefox/77.0
Accept: *//*
Accept-Language: ru-RU,ru;q=0.8,en-US;q=0.5,en;q=0.3
Accept-Encoding: gzip, deflate
X-Requested-With: XMLHttpRequest
Content-Type: multipart/form-data; boundary=---------------------------340475080942671201852026098931
Content-Length: 11069542
Origin: http://dilirink.dlinkddns.com:3491
Connection: keep-alive
Referer: http://dilirink.dlinkddns.com:3491/

-----------------------------340475080942671201852026098931
Content-Disposition: form-data; name="file"; filename="videoplayback.3g2"
Content-Type: video/3gpp2




*/

