/*
    Project: Aufgabe 7
    Author Maximilian Roll / Martin Platajs 5BHEL
    Date: 11.12.2020
*/

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>

#define msgsize 256

char **splitText(char filebuf[])
{
    unsigned int resultCount = 0;
    char ** resArray = NULL;
    char buf[1024];
    char *lineBuffer, *wordBuffer;
    char * line = strtok_r(filebuf, "\n", &lineBuffer);
    while (line) //until end of array
    {
        strcpy(buf, line);
        unsigned short int contLineParse = 1, wordCount = 0;
        char * currentWord = strtok_r(buf, ":", &wordBuffer);
        while ((currentWord)  && contLineParse) //until end of array and active line parsing
        {
            switch(wordCount)
            {
                case 0: // nothing to do
                        break;
                case 1: //check if running, exit loop if not
                    if(strcmp(currentWord, "running"))
                    {
                        contLineParse = 0;
                    }
                    break;
                case 2:
                    //Allocate memory for new array element
                    resArray = realloc (resArray, sizeof (char*) * ++resultCount);
                    if (resArray == NULL)
                    {
                        // something bad happened
                        exit (-1);
                    }

                    //Save word from line in position 3 of line
                    //Allocate permanent storage for saving of value
                    //Strtok reuses wordBuffer!
                    char * saveWord = malloc(strlen(currentWord));
                    strcpy(saveWord,currentWord);
                    //Store in prev allocated memory
                    resArray[resultCount-1] = saveWord;
                    break;
                case 3: //exit loop
                    contLineParse = 0;
                    break;
                default:
                    // nothing to do
                    break;
            }
            wordCount++;
            currentWord = strtok_r(NULL, ":", &wordBuffer); 
        }
    line = strtok_r(NULL, "\n", &lineBuffer);
    }
    // add empty element to indicate array end
    resArray = realloc(resArray,sizeof(char *) * (resultCount + 1));
    resArray[resultCount] = NULL;
    
    return resArray;
}

void readFile(const char filename[],char buffer[],size_t len)
{
    int fd = open(filename,O_RDONLY);
    if((read(fd,buffer,len))!= -1)
    {
        //printf("%s\n", buffer);
    }
    else
    {
        printf("Error couldn't read from file %s\n", filename);
    }
    close(fd);
}

int main(void)
{
    int pid, fd[2];
    char inbuf[msgsize];

    if(pipe(fd) == -1)
    {
        //Exit if pipe fails
        exit(1);
    }
    if((pid = fork()) > 0)
    {
        printf("Child Process: \n");
        //Child Process
        char fileContents[1024];
        char ret[256];
        //get data form file
        readFile("files/data.txt", fileContents, sizeof(fileContents));
        //Split & Filter data from file -> namesArray
        char **namesArray = splitText(fileContents);
        if (namesArray != NULL) { 
            int retOffset = 0;
            memset(ret, 0, sizeof(ret));
            
            //Copy contents of namesArray into ret
            //until end of array
            for (unsigned int cnt = 0; namesArray[cnt] != NULL ; cnt++) 
            {
                printf ("Element %d - %s\n", cnt, namesArray[cnt]); 
                strncat(namesArray[cnt], "\n", 2);  //Add line feed

                //Copy array entry to return array with given offset to last entry
                memcpy(ret + retOffset, namesArray[cnt], strlen(namesArray[cnt]));
                retOffset += strlen(namesArray[cnt]);
            }
        } 
        else 
        {
            //Something went wrong
            printf("Result is NULL \n"); 
        }
        //printf("... sending\n"); debug purposes
        //Write information to pipe
        write(fd[1], ret, msgsize);
        }
    else if(pid < 0)
    {
        printf("Error forking");
    }
    else
    {
        printf("\nMain Process: \n");
        //Main Process
        //Read information from pipe
        while(read(fd[0], inbuf, msgsize))
        {
            //Print Message buffer
            printf("%s \n", inbuf);
        }
    }
    return 0;
}
