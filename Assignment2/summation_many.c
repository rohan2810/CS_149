#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <math.h>

int main(int argc, char *argv[]) {
    double sumOfPowers = 0;

    FILE *fp;
    int fd[2];
    pipe(fd);
    pid_t pid;

    if (argc < 2) {
        exit(0);
    }
    for (int i = 1; i < argc - 1; i++) {
        pid = fork();
        if (pid < 0) printf("Fork failed");
        else if (pid == 0) {
            //child
            close(fd[0]);
            fp = fopen(argv[i], "r");//read the file using fopen and "r" mode
            //if fp does not exist or incase of any error, print cannot open file and
            //throw the error message using perror and exit 1.
            if (fp == NULL) {
                printf("range: cannot open file %s\n", argv[1]);
                perror("Error printed by perror");
                exit(1);
            }
            char input[20];

            //iterate through all the contents of the file using fgets
            while (fgets(input, 20, fp)) {
                if (input[0] != '\n') {
                    //cast the number read from file and the argument to double, and calculate(file^input) and sum all.
                    sumOfPowers += pow(atof(input), atof(argv[argc - 1]));
                } else continue;
            }
            write(fd[1], &sumOfPowers, sizeof(sumOfPowers));
            //close the freader using fclose.
            fclose(fp);
            exit(0);
        }
    }
    close(fd[1]);
    double summation;
    for (int i = 1; i < argc - 1; i++) {
        wait(NULL); // wait until processes are complete
        read(fd[0], &summation, sizeof(summation));
        sumOfPowers += summation;
    }
    printf("%0.2lf\n", sumOfPowers);
    return 0;
}