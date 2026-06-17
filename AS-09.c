/*
Name : Shahad K
Date : 12-06-2026

Objective: 
To understand working of pipe between two processes. 
*/ 

#include <stdio.h>
#include <fcntl.h>          
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>

int main(int argc, char *argv[])
{
    // Validate argument presence
    if(argc < 2)
    {
        printf("Error: No arguments passed Usage: %s <command1> '|' <command2>\n", argv[0]);
        return 1;
    }
    else if(argc < 4)
    {
        printf("Error: Insufficient arguments passed Usage: %s <command1> '|' <command2>\n", argv[0]);
        return 1; 
    }

    int pipe_index = -1;
    for(int i = 0; argv[i]; i++)
    {
        if(strcmp(argv[i], "|") == 0 || strcmp(argv[i], "'|'") == 0)
        {
            pipe_index = i;
            break;
        }
    }

    // Ensure a pipe index token was actually located
    if (pipe_index == -1) {
        printf("Error: Missing pipe token '|' Usage: %s <command1> '|' <command2>\n", argv[0]);
        return 1;
    }

    // 1. Erase the pipe argument entirely by overwriting it with NULL
    argv[pipe_index] = NULL; 

    // 2. Point to command 1 (starts at argv[1])
    char **command1 = &argv[1]; 

    // 3. Point to command 2 (starts right after the pipe slot)
    char **command2 = &argv[pipe_index + 1]; 

    int fd[2];
    
    // Creating a pipe
    if(pipe(fd) == -1)
    {
        perror("pipe");
        return 1;
    }
    
    // --- CHILD 1: EXECUTES FIRST COMMAND (Writes to Pipe) ---
    pid_t child1 = fork();
    if(child1 == -1)
    {
        perror("fork child1");
        return 1;
    }
    else if(child1 == 0)
    {
        // Child 1 reads nothing from the pipe, so close read end
        close(fd[0]);

        // Redirect standard output (1) to the write end of the pipe (fd[1])
        dup2(fd[1], STDOUT_FILENO);
        
        // Clean up the original file descriptor after duplication
        close(fd[1]);

        // Execute first command (e.g., ls)
        execvp(command1[0], command1);
        perror("execvp command1 failed");
        exit(1);
    }

    // --- CHILD 2: EXECUTES SECOND COMMAND (Reads from Pipe) ---
    pid_t child2 = fork();
    if(child2 == -1)
    {
        perror("fork child2");
        return 1;
    }
    else if(child2 == 0)
    {
        // Child 2 writes nothing to the pipe, so close write end
        close(fd[1]);

        // Redirect standard input (0) to the read end of the pipe (fd[0])
        dup2(fd[0], STDIN_FILENO);
        
        // Clean up the original file descriptor after duplication
        close(fd[0]);

        // Execute second command (e.g., wc)
        execvp(command2[0], command2);
        perror("execvp command2 failed");
        exit(1);
    }

    // --- MAIN PARENT PROCESS ---
    close(fd[0]);
    close(fd[1]);

    // Parent waits for both child processes to finish cleanly
    waitpid(child1, NULL, 0);
    waitpid(child2, NULL, 0);

    return 0;
}
