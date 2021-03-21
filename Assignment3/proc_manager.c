#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <time.h>
#include <fcntl.h>


int main(int argc, char *argv[]) {

    if (argc < 2) { // If no files is entered print error
        fprintf(stderr, "No files entered.\n"); // Error
        return 1;
    }
    char *line = NULL;
    size_t len = 0;
    ssize_t read;
    FILE *fp;
    char newString[20][20];
    int i, j, ctr;
    // open file
    fp = fopen(argv[1], "r");

    if (fp == NULL) { // if file does not exist exit with an error
        printf("Can't open file.\n");
        exit(1);
    }
    while ((read = getline(&line, &len, fp)) != -1) {
        printf("---------------------------------------\n");
        printf("Split string by space into words :\n");
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
        printf("\n Strings or words after split by space are :\n");
        for (i = 0; i < ctr; i++)
            printf(" %s\n", newString[i]);
    }
    fclose(fp);
    if (line)
        free(line);
    exit(EXIT_SUCCESS);
}