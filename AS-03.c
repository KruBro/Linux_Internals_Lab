/*
Shahad K
AS-03.c
Objective:
To understand and implement using basic system calls.
 
Requirements:
Using dup or dup2 redirect printf out to a given file instead of printing to stdout.
Pass the file name using command-line arguments.
Try using both system calls (dup and dup2).
Sample Execution:
./dup_dup2 f1.txt
Print this message into STDOUT
 
Note : printf() should be diverted on file and one on STDOUT
  
Date of Submission : 05-31-2026
*/ 

#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>

int main(int argc, char *argv[])
{
    //IF no Arguments
    if(argc < 2)
    {
        printf("Usage : <file_name>.txt\n");
        return 1;
    }

    char *filename = argv[1];
    int is_dup = 0, is_dup2 = 0;

    int choice;
    printf("Do You want to implement : \n1. dup()\n2. dup2()\nOption : ");
    scanf("%d", &choice);
    getchar();

    if(choice == 1)
    {
        is_dup = 1;
    }
    else if(choice == 2)
    {
        is_dup2 = 1;
    }
    else
    {
        printf("Invalid Choice\n");
        return 1;
    }

    char buffer[100];
    printf("Enter the Message you want to write in to output.txt : ");
    scanf("%[^\n]", buffer);
    getchar();

    fflush(stdout);

    //Opening the file
    int file_fd = open(filename, O_CREAT | O_EXCL | O_WRONLY, 0644);
    if(file_fd == -1)
    {
        if(errno == EEXIST)
        {
            file_fd = open(filename, O_WRONLY | O_TRUNC, 0644);

            if(file_fd == -1)
            {
                perror("Failed To Truncate");
                return 1;
            }
        }
        else
        {
            perror("open");
            return 1;
        }
    }


    if(is_dup)
    {
        //Duplicate the stdout
        int stdout_backup  = dup(1);
        if(stdout_backup == -1)
        {
            perror("dup");
            close(file_fd);
            return 1;
        }

        //close stdout, slot 1 becomes free
        close(1);

        //Redirect the file_descriptor to 1
        dup(file_fd);

        printf("%s\n", buffer);
        printf("Using dup()\n");

        fflush(stdout);

        //Restore stdout : close the file at 1, then clone backup in 1.
        close(1);
        if(dup(stdout_backup) == -1)
        {
            perror("Failed to backup");
            return 1;
        }

        close(file_fd);
        close(stdout_backup);
    }
    else if(is_dup2 == 1)
    {
        //Backup stdout to descriptor 10
        if(dup2(1, 10) == -1)
        {
            perror("dup");
            close(file_fd);
            return 1;
        }

        //Overwrite standard output with filel_descriptor
        if(dup2(file_fd, 1) == -1)
        {
            perror("Failed to redirect");
            close(file_fd);
            close(10);
            return 1;
        }

        printf("%s\n", buffer);
        printf("using dup2()\n");

        fflush(stdout);

        //Restore stdout
        if(dup2(10, 1) == -1)
        {
            perror("Redirecting to Stdout failed");
            close(file_fd);
            close(10);
            return 1;
        }

        close(10);
        close(file_fd);
    }

    fflush(stdout);
    printf("SUCCESS!, Output is retored back to the terminal Screen\n");
    return 0;
}