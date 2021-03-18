#include <stdio.h>
#include <stdlib.h>
#include <math.h>

int main(int argc, char *argv[]) {
    double sumOfPowers = 0;

    //exit 0 if the length of arguments is less than 3, i.e. if one of the parameters is missing
    if (argc < 3) {
        exit(0);
    }

    //read the file using fopen and "r" mode
    FILE *fp = fopen(argv[1], "r");

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
            sumOfPowers += pow(atof(input), atof(argv[2]));
        } else continue;
    }

    //print the final sum.
    printf("%0.2lf\n", sumOfPowers);

    //close the freader using fclose.
    fclose(fp);
    return 0;
}