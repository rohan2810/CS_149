#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>

int main(int argc, char *argv[]) {

    double sum = 0;
    int count = 0;
    FILE *fp;
    int fd1[2];
    int fd2[2];
    int fileCount = argc - 1;
    pipe(fd1); //pipe1
    pipe(fd2); //pipe2
    pid_t pid;

    if (argc < 2) {
        exit(0);
    }

    // Creating new child processes and calculating average for particular file
    for (int i = 0; i < fileCount; i++) {

        pid = fork();

        //child process
        if (pid == 0) {
            close(fd2[0]);
            close(fd1[0]);

            fp = fopen(argv[i + 1], "r"); // r is the mode for read

            if (fp == NULL) {
                printf("avg: cannot open input file \n");
                exit(1);
            } else {
                double number;

                while (fscanf(fp, "%lf", &number) != EOF) {
                    sum += number;
                    count++;
                }


                write(fd1[1], &sum, sizeof(sum));
                write(fd2[1], &count, sizeof(count));

                fclose(fp);
                exit(0);
            }
        } else if (pid < 0) {
            printf("Fork failed");
        }
    }

    close(fd1[1]);
    close(fd2[1]);

    double val = 0;
    int counter = 0;

    for (int i = 0; i < fileCount; i++) {
        wait(NULL); // wait until processes are complete

        read(fd1[0], &val, sizeof(val));
        read(fd2[0], &counter, sizeof(counter));
        sum += val;
        count += counter;
    }

    printf("%lf\n", sum / count);
    exit(0);
}



