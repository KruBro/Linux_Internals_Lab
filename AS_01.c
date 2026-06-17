/*
 * A01 - Implement a cp(copy) command with –p option
 *
 * Usage:
 *   ./my_copy source dest
 *   ./my_copy -p source dest
 *
 * Requirements:
 * - If dest exists: ask confirmation "File dest-file already exists. Do you want to overwrite (Y/n)"
 *   If 'n'/'N' => don't overwrite and exit.
 *   If 'Y'/'y' or Enter => overwrite.
 * - Copy should preserve size including 0-sized files.
 * - -p should copy permissions from source to dest (use fstat + fchmod / open mode logic).
 * - Implement: int my_copy(int source_fd, int dest_fd);
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <errno.h>
#include <string.h>

int my_copy(int source_fd, int dest_fd);

static void usage(const char *prog)
{
    fprintf(stderr, "Usage:\n");
    fprintf(stderr, "  %s source_file dest_file\n", prog);
    fprintf(stderr, "  %s -p source_file dest_file\n", prog);
}

int main(int argc, char *argv[])
{
    int preserve = 0;       //Flag to check if "-p" is present or not
    char *src;
    char *dst;

    if(argc != 3 && argc != 4)
    {
        usage(argv[0]);
        return 1;
    }

    if(argc == 4)
    {
        if(strcmp(argv[1], "-p") != 0)
        {
            usage(argv[0]);
            return 1;
        }

        preserve = 1;
        src = argv[2];
        dst = argv[3];
    }
    else
    {
        src = argv[1];
        dst = argv[2];
    }

    int source_fd = open(src, O_RDONLY);

    if(source_fd == -1)
    {
        perror("open source");
        return 1;
    }


    //Open Destination File,
    //Create a new file if file is non-existent
    int dest_fd = open(dst, O_CREAT | O_EXCL | O_WRONLY, 0664);

    if(dest_fd == -1)
    {
        if(errno == EEXIST)
        {
            char choice;

            printf("File %s already exists.\n",
                dst);

            printf("Do you want to overwrite (Y/n): ");

            scanf(" %c", &choice);

            if(choice == 'n' || choice == 'N')
            {
                close(source_fd);
                return 0;
            }

            //Truncating the destination file as per the request
            dest_fd = open(dst, O_WRONLY | O_TRUNC);

            if(dest_fd == -1)
            {
                perror("open");
                close(source_fd);
                return 1;
            }
        }
        else
        {
            perror("open");
            close(source_fd);
            return 1;
        }
    }

    struct stat statbuf;
    mode_t mode = 0664;

    //Setting mode to source file mode
    if(preserve)
    {
        if(fstat(source_fd, &statbuf) == -1)
        {
            perror("fstat");
            close(source_fd);
            return 1;
        }

        mode = statbuf.st_mode & 0777;
    }

    if(my_copy(source_fd, dest_fd) == -1)
    {
        close(source_fd);
        close(dest_fd);
        return 1;
    }

    if(preserve)
    {
        if(fchmod(dest_fd, mode) == -1)
        {
            perror("fchmod");
        }
    }

    close(source_fd);
    close(dest_fd);

    return 0;
}

int my_copy(int source_fd, int dest_fd)
{
    char buffer[1024];

    ssize_t bytes_read;

    while((bytes_read = read(source_fd, buffer, sizeof(buffer))) > 0)
    {
        ssize_t total_written = 0;

        while(total_written < bytes_read)
        {
            ssize_t bytes_written = write(dest_fd, buffer + total_written, bytes_read - total_written);

            if(bytes_written == -1)
            {
                perror("write");
                return -1;
            }

            total_written += bytes_written;
        }
    }

    if(bytes_read == -1)
    {
        perror("read");
        return -1;
    }

    return 0;
}
