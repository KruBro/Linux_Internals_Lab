/*
Name : Shahad K
Date : 01-06-2026
Requirements:
Count the number of words, lines and characters(bytes) from files passed through command line. If more than one files passed, print individual count values and file name + calculate the total of all values and print at the end.
If no file are passed wc will read from std input till end of file(Ctrl + D) then count lines, words and characters(bytes).
Implement a word_count() function where you have to pass fd and 3 integer addresses(pass by refference). Int word_count (int fd, int *lines, int *words, int *bytes);
Word_count function will read from the fd and calculates lines, words and bytes, then stores into respective addresses passed (don’t print values inside function).
Main function will open the files in a loop and call word_count function depends upon number of files passed. Print values after calling functions in main.
*/ 

#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <getopt.h>

void w_count(int fd, int *lineCount, int *wordCount, int *charCount)
{
    ssize_t bytesread;
    char ch;
    int in_word = 0;

    while ((bytesread = read(fd, &ch, 1)) > 0)
    {
        (*charCount)++;

        if (ch == '\n') {
            (*lineCount)++;
        }

        if (ch == ' ' || ch == '\t' || ch == '\n' || ch == '\r') {
            in_word = 0;
        } 
        else if (in_word == 0) {
            in_word = 1;
            (*wordCount)++;
        }
    }
}

// Helper function to handle correct flag combinations and spacing
void print_counts(int l, int w, int c, int lf, int wf, int cf, const char *name)
{
    // If no flags are specified, default behavior is to print all three
    if (lf == 0 && wf == 0 && cf == 0) {
        printf(" %d  %d %d", l, w, c);
    } else {
        if (lf) printf(" %d", l);
        if (wf) printf(" %d", w);
        if (cf) printf(" %d", c);
    }

    // Only print the name if one was provided (skips for stdin)
    if (name != NULL) {
        printf(" %s\n", name);
    } else {
        printf("\n");
    }
}

int main(int argc, char *argv[])
{
    unsigned char word_flag = 0, char_flag = 0, line_flag = 0;
    int word_count = 0, char_count = 0, line_count = 0;
    int total_wc = 0, total_lc = 0, total_bc = 0;
    int opt;

    while((opt = getopt(argc, argv, "lwc")) != -1)
    {
        switch(opt)
        {
            case 'l' : 
                line_flag = 1;
                break;
            case 'w' :
                word_flag = 1;
                break;
            case 'c' :
                char_flag = 1;
                break;
            case '?' :
                fprintf(stderr, "Usage : %s [-l] [-w] [-c] [file ...]\n", argv[0]);
                return 1;
        }
    }

    if(optind == argc)
    {
        w_count(0, &line_count, &word_count, &char_count);
        print_counts(line_count, word_count, char_count, line_flag, word_flag, char_flag, NULL);
    }
    else
    {
        for(int i = optind; i < argc; i++)
        {
            line_count = word_count = char_count = 0;

            int fd = open(argv[i], O_RDONLY);
            if(fd == -1)
            {
                perror(argv[i]); // Passing the filename makes error messages cleaner
                continue;        // Skip broken files
            }

            w_count(fd, &line_count, &word_count, &char_count);
            
            print_counts(line_count, word_count, char_count, line_flag, word_flag, char_flag, argv[i]);

            total_bc += char_count;
            total_wc += word_count;
            total_lc += line_count;

            close(fd);
        }

        if ((argc - optind) > 1) {
            print_counts(total_lc, total_wc, total_bc, line_flag, word_flag, char_flag, "total");
        }
    }
    return 0;
}
