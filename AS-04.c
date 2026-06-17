/*
Name : Shahad K
Date : 08-06-2026
Objective:

To understand and implement using advanced system calls.
Understand the need of file synchronization between processes.
Requirements:

Using fcntl system call synchronize a file between two processes (parent and child process).
Pass the file name using command-line arguments.
Before writing to file check file is locked, in case it is locked must wait the process until its unlocked.
If its unlocked, lock file and continue with writing.
Both process will do the same procedure.
Sample Execution:
1.When no arguments are passed
./a.out
Insufficient arguments
Usage:- ./a.out filename
2. ./a.out f1.txt
PARENT PROCESS: locked file
PARENT PROCESS: writing to file f1.txt
PARENT PROCESS: unlocked file
CHILD PROCESS: locked file
CHILD PROCESS: writing to file f1.txt
CHILD PROCESS: unlocked file
 
Note : Parent will perform some task Ex: Print prime numbers upto 15
Child may perform another task Ex : Print Fibonacci series upto 10
These things should be present in the file
*/ 

#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/wait.h>

int is_prime(int num)
{
    if(num < 2)
    {
        return 0;
    }

    for(int i = 2; i * i <= num; i++)
    {
        if(num % i == 0)
            return 0;
    }

    return 1;
}

void print_fib(int limit)
{
    if(limit == 0)
        return;

    int num1 = 0;
    int num2 = 1;

    for(int i = 0; i < limit; i++)
    {
        printf("%d\n", num1);

        int next_num = num1 + num2;
        num1 = num2;
        num2 = next_num;
    }
}

int main(int argc, char *argv[])
{
    struct flock lock;
    char *file_name = argv[1];
    int fd = open(file_name, O_CREAT | O_EXCL | O_WRONLY, 0644);
    if(fd == -1)
    {
        if(errno == EEXIST)
        {
            printf("File Already Exists\n");
            printf("Truncating...\n");
            fd = open(file_name, O_WRONLY | O_TRUNC);
            if(fd == -1)
            {
                perror("open_trunc");
                return 1;
            }
        }
        else
        {
            perror("open");
            return 1;
        }
    }

    printf("File Has Been Opened Successfully\n");

    int ret = fork();
    if(ret == -1)
    {
        perror("fork");
        close(fd);
        return 1;
    }
    else if(ret == 0)
    {
        //Child
        lock.l_type = F_WRLCK;
        lock.l_len = 0;
        lock.l_start = 0;
        lock.l_whence = SEEK_SET;
        lock.l_pid = getpid();

        //Calling fcntl 
        fcntl(fd, F_SETLKW, &lock);
        printf("Child Locked the File\n");
        dup2(STDOUT_FILENO, 10);
        dup2(fd, 1);

        //Printing Fibonacci Number
        print_fib(10);

        //Redirecting
        fflush(stdout);

        dup2(10, 1);
        close(10);
        lock.l_type = F_UNLCK;
        fcntl(fd, F_SETLKW, &lock);
        printf("Child Released the File\n");
    }
    else if(ret > 0)
    {
        //Parent
        //Initializing the structure members
        lock.l_type = F_WRLCK;
        lock.l_len = 0;
        lock.l_start = 0;
        lock.l_whence = SEEK_SET;
        lock.l_pid = getpid();

        //Calling fcntl 
        fcntl(fd, F_SETLKW, &lock);
        printf("Parent Locked the File\n");
        dup2(STDOUT_FILENO, 10);
        dup2(fd, 1);
        for(int i = 0; i < 15; i++)
        {
            if(is_prime(i))
                printf("%d\n", i);
        }

        //Redirecting
        fflush(stdout);
        dup2(10, 1);
        close(10);
        lock.l_type = F_UNLCK;
        fcntl(fd, F_SETLKW, &lock);
        printf("Parent Released the File\n");

        int wstatus;
        wait(&wstatus);
        if(WIFEXITED(wstatus))
        {
            printf("The Child has been Exited with the status %d\n", WEXITSTATUS(wstatus));
        }
        else 
        {
            printf("The Child has Exited upnormally\n");
        }
    }

    close(fd);
    return 0;
}
