#include "GetRequest.h"
#include "MakeHeader.h"
#include "Routing.h"

#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <sys/socket.h>
#include <netinet/in.h>   // for sockaddr_in, htons, INADDR_ANY
#include <arpa/inet.h>    // for htons(), inet_addr(), etc.
#include <stdlib.h>
#include <sys/param.h>
#include <sys/stat.h>
#include <dirent.h>
#include <ctype.h>

#define READING_FILE_BUFFER_SIZE 1024


void sendResponse(int clientSocket,char *filePath, int inputFile)
{
    /* Sending Header */
    struct stat st;
    stat(filePath,&st);
    unsigned long fileSize=st.st_size;
    char header[180];
    MakeHeader(header,sizeof(header),filePath,fileSize);
    write(clientSocket,header,strlen(header));

    /* Sending Data*/
   unsigned char buffer[READING_FILE_BUFFER_SIZE];
    int totalBytesRead=0;
    while( true)
    {
        int bytesRead =read(inputFile,buffer +totalBytesRead,sizeof(buffer)-totalBytesRead);
        totalBytesRead+=bytesRead;
        if(bytesRead == 0)
        {
            write(clientSocket,buffer,totalBytesRead);
            break;
        }
        if(totalBytesRead >= READING_FILE_BUFFER_SIZE - ( READING_FILE_BUFFER_SIZE/10))
        {
            write(clientSocket,buffer,totalBytesRead);
            totalBytesRead=0;
        }
    }
}

void GetRequest(char method,char *request, char *httpVersion,int clientSocket)
{
    /* Routing */
    Routing(request,request);

    /* Reading request */
    char filePath[2048];
    strcpy(filePath,"./src");
    strcat(filePath,request);
        
    int inputFile=open(filePath,O_RDONLY);
    if(inputFile ==-1)// Requested file does not exist -> return 404
    {
        printf("Error 404\n");
        strcpy(filePath,"./ErrorPages/error404.html");
        inputFile=open(filePath,O_RDONLY);
        if(inputFile == -1)
        {
            printf("Could not find the error file");
            exit(1);
        }
        sendResponse(clientSocket,filePath,inputFile);
    }
    else
    {
        sendResponse(clientSocket,filePath,inputFile);// Response 200
    }
    close(inputFile);
}
