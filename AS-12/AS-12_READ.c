//Read_SHM

#include <stdio.h>
#include <sys/shm.h>
#include <string.h>
#include <unistd.h>

void reverse(char *str)
{
    if(str == NULL || *str == '\0')
        return;
    
    char *start = str;
    char *end = str;

    while(*end)
    {
        end++;
    }
    end--;

    while(start < end)
    {
        char temp = *start;
        *start = *end;
        *end = temp;
        start++;
        end--;
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

    //Copying the data from the ptr
    strcpy(buffer, ptr);

    printf("The data inside SHM is %s\n");

    //Reversing the data inside SHM
    reverse(buffer);

    //write back to the SHM
    strcpy(ptr, buffer);

    if(shmdt(ptr) < 0)
    {
        perror("shmdt");
        return 1;
    }

    printf("The SHM is detached Successfully\n");

    //deleting the shm memory
    if(shmctl(shm_id, IPC_RMID, NULL) < 0)
    {
        perror("shmctl");
        return 1;
    }

    printf("The shm is destroyed successfully\n");

    return 0;
}