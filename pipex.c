/* 
    CS43203: Systems Programming
    Name: Lucas Miller
    Date: 11/10/2020
    Assignment: pipex.c
*/

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>


#define errorHandler(m,x)  {perror(m); exit(x);}

int main(int ac , char **av)
{
    int pipex[3]; //File descriptors
    //int newfd;
    int pid;

    if (ac != 4)
    {
        fprintf(stderr, "usage: pipex cmd1 cmd2 cmd3\n");
        exit(1);
    }

    if (pipe(pipex) == -1)
        errorHandler("Unable to get a pipe", 1);
    
    //creating two new processes
    if( (pid = fork()) == -1)
        errorHandler("Cannot fork", 2);

    if (pid > 0) {
        close(pipex[1]);
        if(dup2(pipex[0], 0) == -1)
            errorHandler("Could not redirect std output", 4);
        
        close(pipex[1]);
        execlp(av[1], av[1], NULL);
        errorHandler(av[1], 5);
    }
}