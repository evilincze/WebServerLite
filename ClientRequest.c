#include "ClientRequest.h"
#include "GetRequest.h"

#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <sys/socket.h>
#include <errno.h>
#include <netinet/in.h>   // for sockaddr_in, htons, INADDR_ANY
#include <arpa/inet.h>    // for htons(), inet_addr(), etc.
#include <stdlib.h>
#include <sys/param.h>
#include <math.h>
#include <sys/stat.h>
#include <dirent.h>
#include <pthread.h>
#include <ctype.h>

#define BUFFER_SIZE 1024


void *ClientRequest(void *args)
{
    int *clientSocket=(int*) args;

    /* Reading request */
    char method='u';
    char request[1024];
    char httpVersion[50];
    unsigned char keywordBuffer[2048];
    int keywordId=0;
    int stage=0;//0 - GET; 1 - REQ; 2 - HTTP ver... the rest is politely ignored

    int endingCounter=0;
    while(true)
    {   
        unsigned char buffer[BUFFER_SIZE];
        int bytesRead=recv(*clientSocket,buffer,sizeof(buffer),0);
        if(bytesRead == 0)
            break;
        for(int i=0;i<bytesRead;i++)
        {
            if(buffer[i] != 32 && stage <= 2)
                keywordBuffer[keywordId++]=buffer[i];
            else
            {
                if(keywordId > 0)//keyword found
                {
                    keywordBuffer[keywordId] ='\0';
                    if(stage ==0)
                    {
                        method=tolower(keywordBuffer[0]);
                        stage++;
                    }
                    else if(stage ==1)
                    {
                        strcpy(request,keywordBuffer);
                        stage++;
                    }
                    else if(stage ==2)
                    {
                        strcpy(httpVersion,keywordBuffer);
                        stage++;
                    }
                    keywordId=0;
                }
            }
            if(endingCounter % 2 == 0 && buffer[i] == '\r')
                endingCounter++;
            else if (endingCounter % 2 == 1 && buffer[i] == '\n')
                endingCounter++;
            else
                endingCounter=0;
            if(endingCounter == 4 )
            {
                if(method == 'g')
                    GetRequest(method,request,httpVersion,*clientSocket);
                keywordId=0;
                stage=0;
                break;
            }
        }
    }
    close(*clientSocket);
    return NULL;
}