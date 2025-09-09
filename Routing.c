#include <stdio.h>
#include "Routing.h"
#include <string.h>

/* Translates url */
void Routing(char *request,char *filePath)
{
    FILE *routingFile = fopen("./ROUTING","r");
    if(routingFile != NULL)
    {
        char line[1024];
        while(fgets(line,sizeof(line),routingFile) != NULL)
        {
            char *hash = strchr(line, '#');
            if (hash) 
            {
                *hash = '\0';  // Ignore comments
            }
            if (strlen(line) <= 2)// Skip empty lines
                continue;

            char key[512];
            char value[512];
            if(sscanf(line,"%511s %511s",key,value) == 2)
            {
                if(strcmp(key,request) == 0)// Match found
                {
                    strcpy(filePath,value);
                    fclose(routingFile);
                    return;
                }
            }
        }
        fclose(routingFile);
    }
}