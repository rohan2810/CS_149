#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>


struct nlist { /* table entry: */
    struct nlist *next; /* next entry in chain */
    int starttime; /* starttime */
    int finishtime; /* finishtime */
    int index; // this is the line index in the input text file */
    int pid;  // the process id. you can use the pid result of wait to lookup in the hashtable */
    char *command; // command. This is good to store for when you decide to restart a command */
};

#define HASHSIZE 101
static struct nlist *hashtab[HASHSIZE]; /* pointer table */
struct nlist *entry_new;
struct nlist *myNode;

/* This is the hash function: form hash value for string s */
/* TODO change to: unsigned hash(int pid) */
/* TODO modify to hash by pid . */
/* You can use a simple hash function: pid % HASHSIZE */
unsigned hash(int pid) {
    return pid % HASHSIZE;
}

/* lookup: look for s in hashtab */
/* TODO change to lookup by pid: struct nlist *lookup(int pid) */
/* TODO modify to search by pid, you won't need strcmp anymore */
/* This is traversing the linked list under a slot of the hash table. The array position to look in is returned by the hash function */
struct nlist *lookup(int pid) {
    struct nlist *np;
    for (np = hashtab[hash(pid)]; np != NULL; np = np->next)
        if (np->pid == pid)
            return np; /* found */
    return NULL; /* not found */
}



/* insert: put (name, defn) in hashtab */
/* TODO: change this to insert in hash table the info for a new pid and its command */
/* TODO: change signature to: struct nlist *insert(char *command, int pid, int index). */
/* This insert returns a nlist node. Thus when you call insert in your main function  */
/* you will save the returned nlist node in a variable (mynode). */
/* Then you can set the starttime and finishtime from your main function: */
/* mynode->starttime = starttime; mynode->finishtime = finishtime; */
struct nlist *insert(char *command, int pid, int index) {
    struct nlist *np;
    unsigned hashval;
    //TODO change to lookup by pid. There are 2 cases:
    if ((np = lookup(pid)) ==
        NULL) { /* case 1: the pid is not found, so you have to create it with malloc. Then you want to set the pid, command and index */
        np = (struct nlist *) malloc(sizeof(*np));
        if (np == NULL || (np->command = strdup(command)) == NULL)
            return NULL;
        np->pid = pid;
        np->command = command;
        np->index = index;
        hashval = hash(pid);
        np->next = hashtab[hashval];
        hashtab[hashval] = np;
    } else { /* case 2: the pid is already there in the hashslot, i.e. lookup found the pid. In this case you can either do nothing, or you may want to set again the command  and index (depends on your implementation). */
        np->index = index;
        np->command = command;
    }
    return np;
}


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
    struct timespec start;
    struct timespec finish;
    entry_new = (struct nlist *) malloc(sizeof(struct nlist));
    myNode = (struct nlist *) malloc(sizeof(struct nlist));
    //new line is recorded from text file
    while (fgets(result, 100, fp)) {
        commandNum++;
        clock_gettime(CLOCK_MONOTONIC, &start);
        int pid = fork();
        if (pid == 0) { //child process
            doExecvp(entry_new->command, commandNum);
            exit(0);
        } else if (pid > 0) {
            //do nothing
            entry_new = insert(result, pid, commandNum);
            entry_new->starttime = (int) start.tv_sec;
            entry_new->command = result;
            entry_new->index = commandNum;

        } else {
            printf("Error in forking.\n");
            exit(1);
        }


    }

    int wpid;
    int status;

    while ((wpid = wait(&status)) > 0) {
        if (wpid > 0) {
            myNode = lookup(wpid);

            char pidArr[10];
            sprintf(pidArr, "%d", wpid);
            char addMe[] = ".out";
            // concatenate
            strcat(pidArr, addMe);

            int f_write = open(pidArr, O_RDWR | O_CREAT | O_TRUNC, 0777);
            dup2(f_write, fileno(stdout));


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

            int elapsed;
            clock_gettime(CLOCK_MONOTONIC, &finish);
            fprintf(stdout, "Starting Command: %d child %d pid of parent %d \n", myNode->index, wpid, getppid());
            elapsed = ((int) finish.tv_sec - myNode->starttime);
            fprintf(stdout, "Finished child %d pid of parent %d\n", myNode->pid, getppid());
            fprintf(stdout, "Finished at %d, runtime duration: %d \n", (int) finish.tv_sec, elapsed);
            fflush(stdout);

            if (elapsed > 2) {
                myNode->starttime = (int) finish.tv_sec;
                wpid = fork();

                if (wpid < 0) { //fork failed
                    fprintf(stderr, "fork failed");
                    exit(1);
                } else if (wpid == 0) {
                    doExecvp(myNode->command, commandNum);
                    exit(0);
                } else {
                    myNode = insert(result, wpid, commandNum);
                    clock_gettime(CLOCK_MONOTONIC, &start); //get time
                    myNode->starttime = (int) start.tv_sec;
                    myNode->command = result;
                    myNode->pid = wpid;
                }
            } else {
                fprintf(stderr, "spawning too fast\n");
            }

            close(f_write);
            close(f_err);
        }
    }

    return 0;
}
