#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <sys/socket.h>
#include <errno.h>
#include <netinet/in.h>   
#include <arpa/inet.h>    
#include <stdlib.h>
#include <sys/param.h>
#include <math.h>
#include <sys/stat.h>
#include <dirent.h>
#include <pthread.h>

#include "ClientRequest.h"

int main()
{
    int mysock =socket(AF_INET,SOCK_STREAM,0);
    struct sockaddr_in myadd;
    myadd.sin_family=AF_INET;
    myadd.sin_port=htons(80);
    myadd.sin_addr.s_addr=INADDR_ANY;

    if(bind(mysock,(struct sockaddr*)&myadd,sizeof(myadd)) == -1) 
    {
        printf("bind error\n");
        exit(1);
    }
    if(listen(mysock,5) == -1)
    {
        printf("listen error\n");
        exit(1);
    }

  
    printf("Server running...\n");
    while(true)
    {
        int newClient=accept(mysock,NULL,NULL);  
        //printf("client accepted\n");
        if(newClient == -1)
            continue; 
        pthread_t newThread;
        pthread_create(&newThread, NULL, ClientRequest, &newClient);  
        pthread_detach(newThread);
    }
    return 0;
}