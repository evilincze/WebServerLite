#include "MakeHeader.h"
#include <string.h>
#include <stdio.h>


void makeHeader(char *header, int headerSize, int fileSize, const char *contentType) {
    snprintf(header, headerSize,
        "HTTP/1.1 200 OK\r\n"
        "Content-Type: %s\r\n"
        "Content-Length: %d\r\n"
        "Connection: close\r\n\r\n",
        contentType, fileSize);
}
char *getFileExtension(char *filename) 
{
    char *dot = strrchr(filename, '.');
    if(!dot || dot == filename) return "";  
    return dot;  
}

void MakeHeader(char *header,int headerSize,char *filePath,unsigned long fileSize)
{
    const char *ext = getFileExtension(filePath);
    if (strcmp(ext, ".html") == 0)
        makeHeader(header,headerSize,fileSize,"text/html");
    if (strcmp(ext, ".jpg") == 0)
        makeHeader(header,headerSize,fileSize,"image/jpeg");
    if (strcmp(ext, ".png") == 0)
        makeHeader(header,headerSize,fileSize,"image/png");
    if (strcmp(ext, ".css") == 0)
        makeHeader(header,headerSize,fileSize,"text/css");
}