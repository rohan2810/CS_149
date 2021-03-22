#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <sys/types.h>


int main(int argc, char *argv[]) {

    if (argc < 2) { // If no files are entered print error
        fprintf(stderr, "No files entered.\n"); // Error
        return 1;
    }

    //initialize all variables
    char *line = NULL;
    size_t len = 0;
    ssize_t read;
    FILE *fp;
    char newString[20][20];
    int i, j, ctr;
    int index = 1;
    // open file of cmdfile.txt in read mode
    fp = fopen(argv[1], "r");

    if (fp == NULL) { // if file does not exist exit with an error
        printf("Can't open file.\n");
        exit(1);
    }
    // loop to read and parse strings
    while ((read = getline(&line, &len, fp)) != -1) {
        for (int i = 0; i < 20; i++)
            for (int j = 0; j < 20; j++)
                newString[i][j] = 0;
        j = 0;
        ctr = 0;
        for (i = 0; i <= (strlen(line)); i++) {
            // if space or NULL found, assign NULL into newString[ctr]
            if (line[i] == ' ' || line[i] == '\0') {
                newString[ctr][j] = '\0';
                ctr++;  //for next word
                j = 0;    //for next word, init index to 0
            } else {
                newString[ctr][j] = line[i];
                j++;
            }
        }
        // printing commands in cmdfile.txt
        for (i = 0; i < ctr; i++)
            printf(" %s\n", newString[i]);
    }
    // close file
    fclose(fp);

    pid_t pid;

    // start fork process
    pid = fork();
    if (pid < 0) { // loop for if the fork fails
        fprintf(stderr, "cannot fork");
        exit(1);
    } else if (pid == 0) { // child process
        // memory allocation using malloc
        char **argv = malloc(10 * sizeof(char *));
        // get elements of the array
        for (j = 0; j < 10; j++) {
            argv[j] = (char *) 0;
            break;
        }
        // *argv is the file being executed, argv is an array of char pointers
        execvp(*argv, argv);
        perror(*argv);
        exit(1);
    } else {  // parent process
        // allocate characters to the out and error file
        // sprintf send formatted output to a string pointed to
        // outFile and errorFile is the pointer to array of char elements where string is stored

        char outFile[20];
        char errorFile[20];
        sprintf(outFile, "%d.out", getpid());
        sprintf(errorFile, "%d.err", getpid());
        // code provided in connect2.c
        int standardOutput = open(outFile, O_CREAT | O_RDWR | O_APPEND, 0777);
        int errorOutput = open(errorFile, O_CREAT | O_RDWR | O_APPEND, 0777);
        // creates a copy to the respective file descriptor
        dup2(standardOutput, 1);
        dup2(errorOutput, 2);

        printf("Starting command %d: child %d pid of parent %d.\n", index, getpid(), getppid());
        printf("Finished child %d pid of parent %d.\n", getpid(), getppid());
        fflush(NULL);
    }
    index = index + 1;

    // given code in lecture
    // used to write the exit code or killed statement to the PID.err
    int status;
    while ((pid = wait(&status)) >= 0) {
        if (pid > 0) {
            // prints exit code of child
            fprintf(stderr, "Exited with exitcode = %d\n", WEXITSTATUS(status));
            // if process is killed prints kill signal
            if (WIFSIGNALED(status)) {
                fprintf(stderr, "Killed with signal %d\n", WTERMSIG(status));
            }
        }
    }
    return 0;
}