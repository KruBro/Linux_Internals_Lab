/*
Name  : Shahad K
Date  : 15-06-2026
Pre-requisites:- 

Knowledge about system calls, How to read and understand ‘man pages’.
Good knowledge about processes and IPC. 
Working of pipe & dup system calls.
Objective: 

• To understand working of pipe between two process. 

Requirements: 

Create three child process and execute commands passed from command-line arguments 
Each command is separated by a '|' (pipe) character. 
First child execute first command (with or without options) and pass o/p to next. 
Second child executes second command (after '|') will reads I/p from first pipe. 
Third Child execute third command (after 2nd pipe '|') will read i/p from second pipe.
Parent will wait for  child process to finish .
Sample execution:

 ./three_pipes (No arguments)
 Error: No arguments passed Usage: ./three_pipes  <command1 > '|'   <command2> '|'  <command3 >       

2. ./pipe ls ­l '|' grep 

 Error: Insufficient arguments passed Usage: ./pipe  <command1 > '|'   <command2> '|'  <command3 >

3. ./pipe ls -­l '|' grep “pattern” '|' wc -­l 

5 5 25
*/ 

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>

int main(int argc, char *argv[])
{
    if(argc < 2)
    {
        printf("Error: No arguments passed Usage: %s  <command1 > '|'   <command2> '|'  <command3 >\n", argv[0]);
        return 1;
    }

    //To track the pipe '|'
    int pipe_count = 0;
    int pipe_indices[2];

    //Store pipe Indexes and increment the count of it
    for(int i = 0; argv[i]; i++)
    {
        if(strcmp(argv[i], "|") == 0)
        {
            if(pipe_count < 2)
                pipe_indices[pipe_count] = i;
            pipe_count++;
        }
    }

    //Check the pipe_count;
    if(pipe_count != 2)
    {
        printf("Error: Insufficient arguments passed Usage: %s  <command1 > '|'   <command2> '|'  <command3 >\n", argv[0]);
        return 1;
    }

    // 1. Check if Command 1 is empty (Pipe 1 is the very first argument)
    if (pipe_indices[0] == 1) 
    {
        printf("Error: Missing first command\n");
        return 1;
    }

    // 2. Check if Command 2 is empty (Pipe 2 immediately follows Pipe 1)
    if (pipe_indices[1] == pipe_indices[0] + 1) 
    {
        printf("Error: Missing second command (empty pipe)\n");
        return 1;
    }

    // 3. Check if Command 3 is empty (Pipe 2 is the final argument)
    // Note: argc - 1 is the index of the last valid argument string
    if (pipe_indices[1] == argc - 1) 
    {
        printf("Error: Missing third command\n");
        return 1;
    }


    //Replace the pipe string in to null
    argv[pipe_indices[0]] = NULL;
    argv[pipe_indices[1]] = NULL;
    
    //Store the commands in the respective way
    char **cmd1 = &argv[1];
    char **cmd2 = &argv[pipe_indices[0] + 1];
    char **cmd3 = &argv[pipe_indices[1] + 1];

    //Create the pipes
    int pipe1[2];
    int pipe2[2];

    if(pipe(pipe1) < 0 || pipe(pipe2) < 0)
    {
        perror("pipe");
        return 1;
    }

    int child1_ret1 = fork();
    if(child1_ret1 == -1)
    {
        perror("fork child1");
        return 1;
    }
    else if(child1_ret1 == 0)
    {
        //Child1
        //redirect write end of the pipe to stdout
        dup2(pipe1[1], STDOUT_FILENO);

        //Close all the unused pipe descriptors in the child
        close(pipe1[0]), close(pipe1[1]);
        close(pipe2[0]), close(pipe2[1]);

        execvp(cmd1[0], cmd1);
        perror("execvp");
        exit(127);
    }

    //Only Parent reaches here
    int child2_ret = fork();

    if(child2_ret == -1)
    {
        perror("fork child2");
        return 1;
    }
    else if(child2_ret == 0)
    {
        //Child 2

        //Read from the pipe 1 and write to the pipe2
        dup2(pipe1[0], STDIN_FILENO);
        dup2(pipe2[1], STDOUT_FILENO);

        //Close all the unused pipe descriptors in the child
        close(pipe1[0]), close(pipe1[1]);
        close(pipe2[0]), close(pipe2[1]);
        
        execvp(cmd2[0], cmd2);
        perror("execvp");
        exit(127);
    }

    //Only the parent reaches here
    int child3_ret = fork();

    if(child3_ret == -1)
    {
        perror("fork");
        return 1;
    }
    else if(child3_ret == 0)
    {
        //Child3 

        //Read from pipe2 read end
        //output naturally to the stdout
        dup2(pipe2[0], STDIN_FILENO);

        //Close all the unused pipe descriptors in the child
        close(pipe1[0]), close(pipe1[1]);
        close(pipe2[0]), close(pipe2[1]);

        execvp(cmd3[0], cmd3);
        perror("execvp");
        exit(127);
    }

    //Only Parent Reaches here

    //Close the pipes
    close(pipe1[0]), close(pipe1[1]);
    close(pipe2[0]), close(pipe2[1]);

    for(int i = 0; i < 3; i++)
    {
        wait(NULL);
    }

    return 0;
}