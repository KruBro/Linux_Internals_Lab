/*
Name : Shahad K
Date : 16-06-2026

Pre-requisites:- 

Knowledge about system calls, How to read and understand ‘man pages’.
Good knowledge about Pipe ,FIFO and Shared memory 
Objective: 

To understand how to establish communication between 3 different process using 3 different IPC mechanism.
Requirements: 

Create 2 different process by executing 2 different a.out (executable) file’s
Create a new process (child process) in Process 1
Create a Pipe between Parent and Child Process
Create SHM from any of the process. Either Process 1 or Process 2
In Parent Process 1 
Prompt the user to enter a string 
Write the string in pipe 
Read the string from pipe into child process
Convert the string into Upper case
Write the Upper case string  in shared memory
6.In process 2

 
Read the data from the shared memory 
Reverse input which is read from SHM
Write the Reverse string  in FIFO
7.Read the data from FIFO in Parent process 1
8.Print the output string on stdout
*/ 

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <errno.h>

int to_upper(char c) {
    if(c >= 'a' && c <= 'z')
        return c - 32;
    return c;
}

void convert_string_to_upper(char *str) {
    if(str == NULL || *str == '\0')
        return;
    while(*str) {
        *str = to_upper(*str);
        str++;
    }
}

int main() {
    char fifo_buffer[100];

    // Create the SHM (using a defined key, 0666 permissions)
    int shm_id = shmget((key_t)12345, 100, IPC_CREAT | 0666);
    if(shm_id < 0) {
        perror("Process 1: shmget failed");
        return 1;
    }

    // Attach the SHM
    char *ptr = (char *)shmat(shm_id, NULL, 0);
    if(ptr == (char *) -1) {
        perror("Process 1: shmat failed");
        return 1;
    }

    // Explicitly initialize shared memory to prevent reading garbage
    memset(ptr, 0, 100);

    // Create the FIFO here first, ensuring it is ready for both processes
    if (mkfifo("my_fifo", 0666) < 0) {
        if (errno != EEXIST) { // If it fails for reasons other than already existing
            perror("Process 1: mkfifo failed");
            return 1;
        }
    }

    // Create the pipe
    int fd[2];
    if(pipe(fd) < 0) {
        perror("pipe failed");
        return 1;
    }

    // Create a child Process
    int child_pid = fork();
    if(child_pid < 0) {
        perror("fork failed");
        return 1;
    }
    
    if(child_pid == 0) {
        // --- CHILD PROCESS ---
        char child_buffer[100];
        close(fd[1]); // Close unused write end

        ssize_t bytes_read = read(fd[0], child_buffer, sizeof(child_buffer) - 1);
        if(bytes_read > 0) {
            child_buffer[bytes_read] = '\0';
            printf("[Child] Read from Parent via Pipe: %s\n", child_buffer);
        }
        close(fd[0]);

        convert_string_to_upper(child_buffer);
        strcpy(ptr, child_buffer); // Write uppercase to SHM
        printf("[Child] Wrote Uppercase to SHM. Terminating.\n");

        exit(0);
    } 
    
    // --- PARENT PROCESS 1 ---
    char parent_message[100];
    close(fd[0]); // Close unused read end

    printf("Enter a String: ");
    if (fgets(parent_message, sizeof(parent_message), stdin) != NULL) {
        // Strip trailing newline character if present
        parent_message[strcspn(parent_message, "\n")] = '\0';
    }

    write(fd[1], parent_message, strlen(parent_message));
    close(fd[1]); // Signal EOF to child

    // Wait for the child to write to SHM and exit
    wait(NULL);

    printf("[Parent] Child finished. Waiting for Process 2 via FIFO...\n");

    // Open FIFO for reading. This blocks safely until Process 2 opens it for writing.
    int fd_fifo = open("my_fifo", O_RDONLY);
    if(fd_fifo < 0) {
        perror("Parent: FIFO open failed");
        return 1;
    }

    // Read the reversed data passed out by Process 2
    ssize_t bytes_read = read(fd_fifo, fifo_buffer, sizeof(fifo_buffer) - 1);
    if(bytes_read >= 0) {
        fifo_buffer[bytes_read] = '\0';
        printf("\n[Final Output] Data received from FIFO: %s\n", fifo_buffer);
    }
    close(fd_fifo);

    // Detach Shared Memory
    shmdt(ptr);

    // Final clean up of filesystem
    unlink("my_fifo");
    return 0;
}
