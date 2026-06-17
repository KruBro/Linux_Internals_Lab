//WRITE_SHM

#include <stdio.h>
#include <sys/shm.h>
#include <string.h>
#include <unistd.h>

int to_upper(char c)
{
    if(c >= 'a' && c <= 'z')
        return c - 32;
    
    return c;
}

void convert_string_to_upper(char *str)
{
    if(str == NULL || *str == '\0')
        return;
    
    while(*str)
    {
        *str = to_upper(*str);
        str++;
    }
}

int main()
{
    char buffer[100];

    //Creating the shared memory
    int shm_id = shmget('A', 100, IPC_CREAT | 0766);

    //Safety Check
    if(shm_id < 0)
    {
        perror("shmget");
        return 1;
    }

    // Attach the shared memory address
    char *ptr = (char *)shmat(shm_id, NULL, 0);
    if(ptr == (char *)-1)
    {
        perror("shmat");
        return 1;
    }

    printf("The data you want to write inside SHM from read_SHM : ");
    scanf("%99s", buffer);

    //Converting the string to upper
    convert_string_to_upper(buffer);

    //Writing the buffer to the shm
    strcpy(ptr, buffer);

    printf("Waiting to recieve data from SHM_READ..\n");
    sleep(10);

    //Reading the shm_memory after reading 
    strcpy(buffer, ptr);

    printf("The Data written by shm_read is %s\n", buffer);

    if(shmdt(ptr) < 0)
    {
        perror("shmdt");
        return 1;
    }

    printf("The SHM is detached Successfully\n");

    return 0;
}