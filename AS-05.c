/*
Name : Shahad K
Date : 12-06-2026
Description :

Create a scenario to make zombie process become orphan, print status of each state.

Pre-requisites:

Knowledge about system calls, How to read and understand ‘man pages’.
Good knowledge about processes, zombie and orphan.
Working of fork system call.
Objective:

• To understand different states of a process.

Requirements: 

Create a child process and print status that process is running
After some time print status that process is zombie state
After some time print zombie process cleared by init.
To print status use help of /proc//status file.
For eg: if child pid is 1234, open file /proc/1234/status and print first 3 lines
      5. If that file is not available means that process is cleared.
 
Sample execution:
 
1. ./zomb_orph
A child created with pid 1234
Name:  ./zomb_orph
State: S (sleeping)
Name:  /zomb_orph   (After some time)
State: Z (zombie)
Process 1234 cleared by init  (After some time)
*/ 
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <stdlib.h>

int print_status(int child_pid)
{
    char file_name[100];
    snprintf(file_name, sizeof(file_name), "/proc/%d/status", child_pid);

    int fd = open(file_name, O_RDONLY);
    if(fd == -1)
    {
        perror("open");
        return -1;
    }

    char ch;
    int new_line_count = 0;
    while(read(fd, &ch, 1) > 0)
    {
        putchar(ch);

        if(ch == '\n')
            new_line_count++;
        
        if(new_line_count == 6)
            break;
    }

    close(fd);
    return 0;
}

int main()
{
    pid_t child1_pid = fork();
    if(child1_pid == -1)
    {
        perror("fork");
        return 1;
    }
    else if(child1_pid == 0)
    {
        //Step 1 Child 1 goes to sleep for 5 seconds;
        sleep(5);

        //Exits naturally to become zombie
        exit(0);
    }
    else if(child1_pid > 0)
    {
        printf("A child is created with pid -> %d\n", child1_pid);

        sleep(1);

        //print the current status of the child
        if(print_status(child1_pid) == -1)
        {
            printf("The child1 is cleared by init process\n");
        }

        //Let the parent sleep for another 5 seconds
        //Child became zombie now
        sleep(5);

        //Print Current status of the child1
        //If the child one is already exited, that mean it is cleared by the root init
        printf("After sometime\n");
        if(print_status(child1_pid) == -1)
        {
            printf("The child1 is cleared by init process\n");
        }

        //Let the parent finish some busy task and create a child 2 
        sleep(2);

        pid_t child2_pid = fork();

        if(child2_pid == -1)
        {
            perror("fork");
            return 1;
        }
        else if(child2_pid == 0)
        {
            pid_t ppid1 = getppid();

            //wait for the parent to finish tasks;
            sleep(2);

            pid_t ppid2 = getppid();

            if(ppid1 != ppid2)
            {
                printf("Child2 is cleared by init Process\n");
            }
            else
            {
                printf("It is not yet cleared\n");
            }

            exit(0);
        }
        else if(child2_pid > 0)
        {
            //Parent finishes execution without calling wait() for child 1 or child 2
            //this turn child 1 into an orphan zombie and child 2 into an orphan living process
            sleep(1);
        }
    }

    return 0;
}