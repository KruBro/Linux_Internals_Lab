/*
Name : Shahad K
Date : 12-06-2026

WAP to avoid a child become zombie with out blocking the parent.

Pre-requisites:

Knowledge about system calls, How to read and understand ‘man pages’.
Good knowledge about processes & zombie process.
Working of fork & wait system call.
Objective:

To understand different states of a process.

Requirements: 

Create a child process avoid it become a zombie.
To avoid zombie we need to call wait(), but this block parent until child terminates.
So we need to use waitpid() with proper arguments (Read man page). 
When child is working parent has to continuously print some message.
When ever child terminates parent has to print child terminated and print exit status of child process
Sample execution:
./nonblock_wait
A child created with pid 1234
parent is running
parent is running
parent is running . .
............................
Child 1234 terminated normally with exit status 0 parent terminating
*/ 

#include <stdio.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>

int main()
{
    //Creating a process 
    int ret = fork();

    //Safety Check
    if(ret == -1)
    {
        perror("fork");
        return 1;
    }
    else if(ret == 0)
    {
        //Child
        sleep(5);

        //After 5 seconds, Child Exits Normally
        exit(0);
    }
    else if(ret > 0)
    {
        //Parent;
        //Parent has will continue its process while waiting for the child to terminate;

        int wstatus;
        while(waitpid(ret, &wstatus, WNOHANG) == 0)
        {
            printf("Parent is Running....\n");

            //To avoid frequent Printing
            sleep(1);
        }


        //To check the status of 
        if(WIFEXITED(wstatus))
        {
            printf("The child exited with the status %d\n", WEXITSTATUS(wstatus));
        }
        else
        {
            printf("The child exited upnormally\n");
        }
    }

    return 0;
}