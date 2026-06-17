/*

Name : Shahad K
DATE : 03-06-2026

Objective: 
• To understand how to use fork system calls.
 Requirements: 
Create three child process from same parent. 
Parent has to wait for all three child process.
Print exit status of each child when they terminates.
Sample execution: 
./three_child 
Child 1 with pid 2020 created 
Child 2 with pid 2021 created
Child 3 with pid 2022 created 
Child 2020 is terminated with code 0 
Child 2021 is terminated with code 0 
Child 2022 is terminated with code 0
*/ 

#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>

int main()
{
    pid_t ret_fork1, ret_fork2, ret_fork3;

    // --- CHILD 1 CREATION ---
    ret_fork1 = fork();
    if (ret_fork1 == -1) {
        perror("child1->fork()");
        return 1;
    }
    else if (ret_fork1 == 0) {
        // Child 1 execution path
        printf("Processing Child 1, with pid %d\n", getpid());
        printf("Parent PID -> %d\n", getppid());
        return 0; 
    }

    // --- CHILD 2 CREATION (Only parent reaches here) ---
    ret_fork2 = fork();
    if (ret_fork2 == -1) {
        perror("child2->fork()");
        return 1;
    }
    else if (ret_fork2 == 0) {
        // Child 2 execution path
        printf("Processing Child 2, with pid %d\n", getpid());
        printf("Parent PID -> %d\n", getppid());
        return 0; 
    }

    // --- CHILD 3 CREATION (Only parent reaches here) ---
    ret_fork3 = fork();
    if (ret_fork3 == -1) {
        perror("child3->fork()");
        return 1;
    }
    else if (ret_fork3 == 0) {
        // Child 3 execution path
        printf("Processing Child 3, with pid %d\n", getpid());
        printf("Parent PID -> %d\n", getppid());
        return 0;
    }

    // --- PARENT PROCESS CLEAN-UP ---
    int wstatus;
    for (int i = 0; i < 3; i++) {
        // wait(NULL) blocks until any one child exits
        pid_t exited_pid = wait(&wstatus); 
        
        if (WIFEXITED(wstatus)) {
            // WEXITSTATUS to print the actual exit code
            printf("Child with PID %d exited normally with status %d\n", 
                   exited_pid, WEXITSTATUS(wstatus));
        }
        else {
            printf("Child with PID %d exited abnormally\n", exited_pid);
        }  
    }

    printf("Parent process finished.\n");
    return 0;
}
