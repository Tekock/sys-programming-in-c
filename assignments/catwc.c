#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/unistd.h>
#include <fcntl.h>
#include <string.h>
#define OUT 0
#define IN 1
#define MAXLINE 4096


unsigned countWords(char *str) {
    int state = OUT;
    unsigned word_c = 0;  // word count 
    while (*str)
    {
        if (*str == ' ' || *str == '\n' || *str == '\t') {
            state = OUT;
        }
        else if (state == OUT)
        {
            state = IN;
            ++word_c;
        }
 
        // Move to next character
        ++str;
    }
    return word_c;
}

unsigned countLines(char *str) {
    int state = OUT;  
    unsigned line_c = 0; // line count
 
    while (*str) {
        if (*str == '\n') {
            ++line_c;
        }
        ++str;
    }
    return line_c;
}



int main(int argc, char *argv[]) {

    char* path = argv[1];
    int size;
    pid_t pid;

    int fd[2];
    int k = pipe(fd);

    if (k <0) {
        printf("Error piping.");
        exit(0);
    }

    char line[MAXLINE];
    if ((pid = fork()) < 0) {
        printf("fork failed");
    } else if (pid > 0) {
        printf("Parent process\n");
        int fdf = open(path, O_RDONLY);
        size = read(fdf, line, MAXLINE);
        close(fd[0]);
        write(fd[1], line, size);
    } else {
        printf("Child process\n");
        close(fd[1]);
        int sz = read(fd[0], line, MAXLINE);
        char res[MAXLINE];
        sprintf(res, "%d", sz);
        printf("Got this far - 1 %s\n", res);
        int wc_u = countWords(line);
        char wc[MAXLINE];
        sprintf(wc, "%d", wc_u);
        printf("Got this far - 2 %s\n", wc); 
        int lc_u = countLines(line);
        char lc[MAXLINE];
        sprintf(lc, "%d", lc_u);
        printf("Got this far - 3 %s\n", lc);

    } exit(0);
    
}
