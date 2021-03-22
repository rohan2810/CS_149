#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <sys/wait.h>
#include <fcntl.h>

int main(int argc, char *argv[]) {
    // template code provided in a3/a4
    FILE *fp;
    char *line = NULL;
    size_t len = 0;
    ssize_t read;
    int count = 0;
    int i, j, ctr;

    // read the file
    fp = fopen(argv[1], "r");
    // count number of lines in file
    while ((read = getline(&line, &len, fp)) != -1) {
        count++;
    }

    fclose(fp);

    int l = 0, num = 0;

    // loop to compare the child process to parent processes and read each line
    while (num < count) {
        l = fork();
        num++;

        // loop for child process to read every line
        if (l == 0) {
            int length = 0, characters = 0;
            fp = fopen(argv[1], "r");

            //get the correct line for the child
            for (int i = 0; i < num; i++) {
                if ((read = getline(&line, &len, fp)) == -1) {
                    printf("read failed: %i\n", num);
                    return 1;
                }
            }
            //ctr stores the amount of words in the 2d array
            j = 0;
            ctr = 0;

            for (i = 0; i <= (strlen(line)); i++) {
                // if space or NULL found, assign NULL into newString[ctr]
                if (line[i] == ' ' || line[i] == '\0') {
                    ctr++;  //for next word
                    length = (j > length) ? j + 1 : length;
                    characters = (ctr > characters) ? ctr : characters;
                    j = 0;    //for next word, init index to 0
                } else {
                    j++;
                }
            }
            fclose(fp);

            fp = fopen(argv[1], "r");
            if (fp == NULL) {
                printf("File not found");
                exit(EXIT_FAILURE);
            }

            char newString[20][20];

            j = 0;
            ctr = 0;

            for (i = 0; i <= (strlen(line)); i++) {
                // if space or NULL found, assign NULL into newString[ctr]
                if (line[i] == ' ' || line[i] == '\0' || line[i] == '\n') {
                    newString[ctr][j] = '\0';
                    ctr++;  //for next word
                    j = 0;    //for next word, init index to 0
                } else {
                    newString[ctr][j] = line[i];
                    j++;
                }
            }

            char *argv[characters + 1];
            for (int i = 0; i < characters; i++) {
                argv[i] = (char *) &newString[i][0];
            }
            argv[characters] = (char *) 0;

            fclose(fp);
            if (line) {
                free(line);
            }

            // create char array of an outFile1 and errorFile1
            // read out to outFile1 with the PID.out and errorFile1 with PID.err
            char outFile1[20];
            char errorFile1[20];
            sprintf(outFile1, "%d.out", getpid());
            sprintf(errorFile1, "%d.err", getpid());

            //template code provided in connect2.c
            int outFile = open(outFile1, O_RDWR | O_CREAT | O_APPEND, 0777);
            if (outFile == -1) {
                return 2;
            }

            //template code provided in connect2.c
            int errorFile = open(errorFile1, O_RDWR | O_CREAT | O_APPEND, 0777);
            if (errorFile == -1) {
                return 2;
            }

            fflush(stdout);

            // use dup2 to create a copy of the old file descriptor
            dup2(outFile, 1);
            dup2(errorFile, 2);

            printf("Starting command %i: child %i pid of parent %i\n", num, getpid(), getppid());
            fflush(stdout);
            close(outFile);
            close(errorFile);

            if (execvp(argv[0], argv) == -1) {
                dup2(errorFile, 1);
                perror(argv[0]);
                return 2;
            }
        } else {
            // template code provided in lecture slides of wait
            int pid2, status;
            while ((pid2 = wait(&status)) > 0) {

                if (WIFEXITED(status)) {
                    int statusOfProcess = WEXITSTATUS(status);
                    if (statusOfProcess == 0) {

                        int cpid = pid2;

                        char outFile1[20];
                        char errorFile1[20];
                        sprintf(outFile1, "%d.out", cpid);
                        sprintf(errorFile1, "%d.err", cpid);

                        FILE *outFile = fopen(outFile1, "a+b");
                        if (outFile == NULL) {
                            return 2;
                        }

                        FILE *errorFile = fopen(errorFile1, "a+b");
                        if (errorFile == NULL) {
                            return 2;
                        }

                        fprintf(outFile, "Finished child %i pid of parent %i", cpid, getpid());
                        fprintf(errorFile, "Exited with exitcode = %i", status);


                        fclose(outFile);
                        fclose(errorFile);
                    }
                } else if (WIFSIGNALED(status)) {
                    int cpid = pid2;

                    char outFile1[20];
                    char errorFile1[20];
                    sprintf(outFile1, "%d.out", cpid);
                    sprintf(errorFile1, "%d.err", cpid);

                    FILE *outFile = fopen(outFile1, "a+b");
                    if (outFile == NULL) {
                        return 2;
                    }
                    FILE *errorFile = fopen(errorFile1, "a+b");
                    if (errorFile == NULL) {
                        return 2;
                    }
                    fprintf(outFile, "Finished child %i pid of parent %i", cpid, getpid());
                    fprintf(errorFile, "Killed with signal %i", WTERMSIG(status));

                    fclose(outFile);
                    fclose(errorFile);
                } else {
                    return 2;
                }
            }
        }
    }


    exit(0);
}