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
    int pidOne, pidTwo;

    if (ac != 4)
    {
        fprintf(stderr, "usage: pipex cmd1 cmd2 cmd3\n");
        exit(1);
    }

    if (pipe(pipex) == -1)
        errorHandler("Unable to get a pipe", 1);
    
    //creating two new processes
    if( (pidOne = fork()) == -1)
        errorHandler("Cannot fork", 2);
    

    if (pidOne > 0) {   /* Parent executes av[3] */
        close(pipex[1]);/* Parent doesnt write to pipe */
        if(dup2(pipex[0], 0) == -1)
            errorHandler("Could not redirect std in", 3);
        
        close(pipex[0]); /* stdin is duplicated so we close pipe */
        execlp(av[3], av[3], NULL);
        errorHandler(av[3], 4);

        if( (pidTwo = fork()) == -1)
            errorHandler("Cannot fork", 2);
    
        if (pidOne > 0) {   /* Parent executes av[2] */
            if(dup2(pipex[1], 1) == -1)
                errorHandler("Could not redirect std in", 3);
        
            close(pipex[0]); 
            execlp(av[2], av[2], NULL);
            errorHandler(av[2], 4);
        }
    }

    close(pipex[0]);

    if(dup2(pipex[1], 0) == -1)
        errorHandler("Could not redirect std out", 4);
        
    close(pipex[0]); 
    execlp(av[1], av[1], NULL);
    errorHandler(av[1], 5);
    

}