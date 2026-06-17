#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <errno.h>

void reverse(char *str) {
    if(str == NULL || *str == '\0')
        return;
    
    char *start = str;
    char *end = str;
    while(*end) {
        end++;
    }
    end--;

    while(start < end) {
        char temp = *start;
        *start = *end;
        *end = temp;
        start++;
        end--;
    }
}

int main() {
    char fifo_buffer[100];

    // Connect to the exact same SHM segment using key 12345
    int shm_id = shmget((key_t)12345, 100, 0666);
    if(shm_id < 0) {
        perror("Process 2: shmget failed. Is Process 1 running?");
        return 1;
    }

    // Attach to the segment
    char *ptr = (char *)shmat(shm_id, NULL, 0);
    if(ptr == (char *)-1) {
        perror("Process 2: shmat failed");
        return 1;
    }

    printf("[Process 2] Polling SHM for data...\n");
    // Busy-wait polling loop until Process 1's child writes data into the block
    while (strlen(ptr) == 0) {
        usleep(100000); // Sleep for 100ms to avoid pegging CPU usage
    }

    // Copy data safely out of SHM
    strcpy(fifo_buffer, ptr);
    printf("[Process 2] Read from SHM: %s\n", fifo_buffer);

    // Reverse the string
    reverse(fifo_buffer);

    // Open FIFO for writing. Blocks until Process 1 Parent opens it for reading.
    int fd_fifo = open("my_fifo", O_WRONLY);
    if(fd_fifo < 0) {
        perror("Process 2: FIFO open failed");
        return 1;
    }

    // Write the payload down the pipe line
    write(fd_fifo, fifo_buffer, strlen(fifo_buffer));
    close(fd_fifo);
    printf("[Process 2] Sent reversed string down FIFO.\n");

    // Detach and clean up system resources safely
    shmdt(ptr);
    shmctl(shm_id, IPC_RMID, NULL); 
    printf("[Process 2] Shared memory cleared from OS kernel.\n");

    return 0;
}
