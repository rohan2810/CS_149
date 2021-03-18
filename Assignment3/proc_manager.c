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
    return 0;

}
