/*
Name : Shahad K
Date : 23-06-2026

A17 - WAP to avoid child become zombie by using signal handlers

Type of work:  Individual work
Pre-requisites:

• Knowledge about system calls, How to read and understand ‘man pages’. 
• Good knowledge about signals and signal handling. 
• Working of sigaction system call. 

Objective: 

• To understand, how to avoid zombie asynchronously . 

Requirements: 

 Write two separate programs for both methods.

 Method 1 

1. Create a child process. 
2. Write a signal handler function for SIGCHLD to avoid child become zombie (Do man 7 signal for SIGCHLD) . 
3. Write code inside handler to avoid zombie and print child exit status. 

  Method 2

 1. Create a child process. 
 2. Use sa_flag from struct sigaction to avoid zombie (Refer man ). 
 3. Prints the child exit status inside handler.

Sample execution:  


1. ./avoid_zombie 
child  terminated with exit code 0.
*/ 
#define _XOPEN_SOURCE 700 
#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>

// Standard signal handler using sa_handler
void signal_handler(int signum)
{
    int wstatus;
    // waitpid reaps the child and gets its exit status
    pid_t pid = waitpid(-1, &wstatus, WNOHANG);
    
    if (pid > 0 && WIFEXITED(wstatus))
    {
        printf("child terminated with exit code %d.\n", WEXITSTATUS(wstatus));
    }
}

int main()
{
    struct sigaction sa;
    sa.sa_handler = signal_handler; // Use standard handler
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;                // No special flags needed here

    if(sigaction(SIGCHLD, &sa, NULL) == -1)
    {
        perror("sigaction");
        return 1;
    }

    pid_t ret = fork();
    if(ret < 0)
    {
        perror("fork");
        return 1;
    }
    else if(ret == 0)
    {
        // Child Process
        for(int i = 0; i < 5; i++)
        {
            printf("Child -> PID %d Processing\n", getpid());
            sleep(1);
        }
        exit(0); 
    }

    // Parent Process
    for(int i = 0; i < 10; i++)
    {
        printf("Parent PID-> %d is processing\n", getpid());
        sleep(1);
    }
    return 0;
}
