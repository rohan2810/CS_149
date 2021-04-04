//
// Created on 3/17/21.
// Sample solution
//
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/wait.h>

//helper method to do execvp
void doExecvp(char lineToParse[], int commandNum) {

    //make name of the file
    int pid = getpid();
    char pidArr[10];

    //convert int to string
    sprintf(pidArr, "%d", pid);

    //concatenate pidArr with string ".out"
    char addMe[] = ".out";
    strcat(pidArr, addMe);

    int f_write = open(pidArr, O_RDWR | O_CREAT | O_TRUNC, 0777);
    //creates a copy of the file descriptor f_write
    dup2(f_write, fileno(stdout));

    fprintf(stdout, "Starting command %d: child %d pid of parent %d\n", commandNum, getpid(), getppid());
    //move the buffered data to console
    fflush(stdout);

    //preparing to parse string to pass into execvp arguments
    char *input[strlen(lineToParse)];

    char *token = strtok(lineToParse, " \n");
    int index = 0;

    //parsing
    while (token != NULL) {
        input[index] = token;
        index++;
        token = strtok(NULL, " \n");
    }
    input[index] = NULL;

    //printing to the out file
    fprintf(stdout, "Finished child %d pid of parent %d\n", getpid(), getppid());
    fflush(stdout);
    close(f_write);

    //call execvp
    execvp(input[0], input);

    //Execvp fails: ----------------------------------------
    //make name of the file
    char pidArrforError[10];
    //convert int pid to string
    sprintf(pidArrforError, "%d", pid);
    char addMeError[] = ".err";
    // concatenate
    strcat(pidArrforError, addMeError);

    int f_err = open(pidArrforError, O_RDWR | O_CREAT | O_TRUNC, 0777);
    dup2(f_err, fileno(stderr));

    perror("Execvp failed\n");

    close(f_err);

}

int main(int argc, const char *argv[]) {

    //argc at least = 2; 1 for name of the file (proc_manager), other is input file
    if (argc < 2) {
        printf("Incorrect input!\n");
        return 1;
    }

    // Open file
    FILE *fp = fopen(argv[1], "r");
    if (fp == NULL) {
        printf("Null file.\n");
        return 1;
    }

    char result[100];
    int commandNum = 0;
    //new line is recorded from text file
    while (fgets(result, 100, fp)) {
        commandNum++;
        int pid = fork();
        if (pid == 0) { //child process
            doExecvp(result, commandNum);
            exit(0);
        } else if (pid > 0) {
            //do nothing
        } else {
            printf("Error in forking.\n");
            exit(1);
        }


    }

    int wpid;
    int status;

    while ((wpid = wait(&status)) > 0) {
        //make name of the file
        char pidArrforError[10];
        //convert int pid to string
        sprintf(pidArrforError, "%d", wpid);
        char addMeError[] = ".err";
        // concatenate
        strcat(pidArrforError, addMeError);


        int f_err = open(pidArrforError, O_RDWR | O_CREAT | O_TRUNC, 0777);
        dup2(f_err, fileno(stderr));
        //Normal termination with exit code
        if (WIFEXITED(status)) {
            fprintf(stderr, "Exited with exitcode = %d\n", WEXITSTATUS(status));
        }
            //Abnormal Termination with exit signal
        else if (WIFSIGNALED(status)) {
            fprintf(stderr, "Killed with signal = %d\n", WTERMSIG(status));
        }

        close(f_err);
    }

    return 0;
}
