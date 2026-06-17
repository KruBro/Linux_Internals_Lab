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
}

int main()
{
    Printf("Reciever pid is %d\n", getpid());

    struct sigaction new_act;
    
    memset(&new_act, 0, sizeof(new_act));

    new_act.sa_sigaction = signal_handler;
    new_act.sa_flags = SA_SIGINFO;
    
    sigaddset(&new_act.sa_mask, SIGINT);
    sigaddset(&new_act.sa_mask, SIGTSTP);

    sigaction(SIGINT, &new_act, NULL);
    sigaction(SIGTSTP, &new_act, NULL);

    while(1)
    {
        pause();
    }
}