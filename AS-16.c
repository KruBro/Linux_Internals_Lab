/*
Name : Shahad K
DATE : 22-06-2026

Pre-requisites:

• Knowledge about system calls, How to read and understand ‘man pages’. 
• Good knowledge about signals and signal handling.
 • Working of sigaction system call and signal masking. 
Objective: 

 To understand importance of signal masking. 
Requirements: 

1. Write a signal handler function for any signal, say SIGINT .
2. While its running inside handler (use loop) block other signals(atleast 3) being received . 
3. Use sa_mask from struct sigaction to set signals to be blocked (Refer man ). 
4. To create a signal set use variable of sigset_t. 
5. To add or remove signals from set use sigaddset, sigdelset functions (refer man). 
6. Process will receive blocked signals once its out from handler. 

Sample execution: 

 1. ./block_signal
      Process 2345 waiting for signal.. press ctrl + c from terminal. 
      SIGINT received  
Inside handler 
Inside handler 
. .
.....

2. Now open another terminal and send signal to process using kill command. 
Bash$ kill ­SIGUSR1 2345 
Bash$ kill ­SIGTERM 2345 
Bash$ kill ­SIGABRT 2345 
 
3. After exiting from handler will respond to blocked signal.
*/ 
#define _XOPEN_SOURCE 700 
#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <string.h>

void signal_handler(int signum, siginfo_t *Info, void *data)
{
    for(int i = 0; i < 10; i++)
    {
        printf("Inside the handler\n");
        printf("Signal number is %d\n", signum);
        sleep(1);
    }
    printf("Leaving Handler\n");
}

int main()
{
    printf("Reciever pid is %d\n", getpid());

    struct sigaction new_act;
    sigset_t block_set;

    //Initialize the signal set to be empty
    sigemptyset(&block_set);

    //Add atleast 3 signals 
    sigaddset(&block_set, SIGUSR1);
    sigaddset(&block_set, SIGTERM);
    sigaddset(&block_set, SIGABRT);

    memset(&new_act, 0, sizeof(new_act));

    new_act.sa_sigaction = signal_handler;
    new_act.sa_mask = block_set;
    new_act.sa_flags = 0;
    
    if(sigaction(SIGINT, &new_act, NULL) == -1)
    {
        perror("Error Registering Sigaction");
        return 1;
    }

    while(1)
    {
        pause();
    }
}