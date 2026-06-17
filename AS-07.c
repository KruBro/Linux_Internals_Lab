/*

Name : Shahad K
Date : 03-06-2026
Assignment 07

Requirements:
Create child and execute a command passed from command-line arguments.  
If no arguments passed print a usage message.
In case any wrong command passed, print an error message.
After child terminates print the exit status of child process.
Sample execution: -
1. No args passed (Print usage info)
./exe_child Usage: ./exe_child args...
2. Valid arg. passed
./exe_child date
This is the CHILD process, with id 11612
Wed Apr  4 13:27:19 IST 2012
Child exited with status 0
*/ 

#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main(int argc, char *argv[])
{
    if(argc < 2)
    {
        printf("Usage : %s", argv[0]);
        return 1;
    }

    char *file_name = argv[1];
    int ret = fork();
    if(ret == -1)
    {
        perror("error");
        return 1;
    }
    else if(ret == 0)
    {
        // Child Process
        printf("Child Process id =  %d\n", getpid());
        
        execvp(file_name, &argv[1]);
        
        // This line is ONLY reached if execvp fails
        printf("%s is an invalid command\n", argv[1]); 
        return 1; 
    }

    else if(ret > 0)
    {
        //Parent Process
        int wstatus;
        int ret_wait = wait(&wstatus);
        if(WIFEXITED(wstatus))
        {
            printf("Child exited with the status %d\n", WIFEXITED(wstatus));
        } 
        else
        {
            printf("Child exited Upnormally");
        }
    }

    return 0;
}