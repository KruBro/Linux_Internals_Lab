/*
Name : Shahad K
Date : 22-06-2026

Pre-requisites:

Knowledge about system calls, How to read and understand ‘man pages’.
Good knowledge about signals and signal handling.
Working of sigaction system calls. 
Objective: 

• To understand working of signal handling. 

Requirements: 

Write a signal handler function to print address which caused seg-fault(SIGSEGV). 
Use sigaction system call to register signal. 
Use struct siginfo from sa_sigaction to print address (Read man page). 
Create a segmentation fault from code. 
When error occurs program should print address and exit. 
Sample execution: 

 ./sigsegv 

Segmentation fault ..!! 

Address 0x123456 caused error
*/ 


#define _XOPEN_SOURCE 700 
#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>

void signal_handler(int signum, siginfo_t *info, void *data)
{
    printf("Segfaul\n");
    printf("Address %p Caused Error\n", info->si_addr);
    exit(EXIT_FAILURE);
}

int main()
{
    printf("Processing PID - > %d\n", getpid());

    int is_called = 1;

    struct sigaction sa;

    memset(&sa, 0, sizeof(sa));

    sa.sa_sigaction = signal_handler;
    sa.sa_flags = SA_SIGINFO;

    if(sigaction(SIGSEGV, &sa, NULL) == -1)
    {
        perror("sigaction");
        return 1;
    }

    int *ptr = NULL;
    *ptr = 10;

    return 0;
}