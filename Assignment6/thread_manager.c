#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <stdbool.h>
#include <time.h>

/*****************************************
//CS149 Spring 2021
//Template for assignment 6
//San Jose State University
//originally prepared by Bill Andreopoulos
*****************************************/


//thread mutex lock for access to the log index
//TODO you need to actually use this
pthread_mutex_t tlock1 = PTHREAD_MUTEX_INITIALIZER;

//thread mutex lock for critical sections of allocating THREADDATA
//TODO you need to actually use this
pthread_mutex_t tlock2 = PTHREAD_MUTEX_INITIALIZER;

//thread mutex lock for access to a linked list head
//TODO you need to actually use this
pthread_mutex_t tlock3 = PTHREAD_MUTEX_INITIALIZER;


void *thread_runner(void *);

pthread_t tid1, tid2;

//struct points to the thread that created the object.
//This is useful for you to know which is thread1. Later thread2 will deallocate.
struct THREADDATA_STRUCT {
    pthread_t creator;
};
typedef struct THREADDATA_STRUCT THREADDATA;

// linked list
typedef struct linked_list {
    char string[20];
    struct linked_list *next;
} node;

THREADDATA *p = NULL;
node *head = NULL;


//variable for indexing of messages by the logging function.
int logindex = 0;
int *logip = &logindex;

// variables for input plus the time and date for the message printed out
char buf[20];
char totalTimeAndDate[50];


//A flag to indicate if the reading of input lines by one thread is complete,
//so the other thread knows when to stop.
bool is_reading_complete = false;


// helper functions implemented below
void freeLinkedList();

void insert(char *buf);

char *printDateAndTime();

/*********************************************************
// function main
*********************************************************/
int main() {

    printf("create first thread\n");
    pthread_create(&tid1, NULL, thread_runner, NULL);

    printf("create second thread\n");
    pthread_create(&tid2, NULL, thread_runner, NULL);

    printf("wait for first thread to exit\n");
    pthread_join(tid1, NULL);
    printf("first thread exited\n");

    printf("wait for second thread to exit\n");
    pthread_join(tid2, NULL);
    printf("second thread exited\n");

    exit(0);

}//end main


/**********************************************************************
// function thread_runner runs inside each thread
**********************************************************************/
void *thread_runner(void *x) {
    pthread_t me;

    me = pthread_self();
    printf("This is thread %ld (p=%p)\n", me, p);

    pthread_mutex_lock(&tlock2); // critical section starts
    if (p == NULL) {
        p = (THREADDATA *) malloc(sizeof(THREADDATA));
        p->creator = me;
    }
    pthread_mutex_unlock(&tlock2);  // critical section ends

    if (p != NULL && p->creator == me) {
        printf("This is thread %ld and I created the THREADDATA %p\n", me, p);
        //TODO implement any thread1 functionality.
        //Make sure to use any mutex locks appropriately

        // user helper functions to get date and time
        printDateAndTime();
        while (fgets(buf, 21, stdin) != NULL) {
            // use both locks as it we are accessing both log index as well as linked list
            pthread_mutex_lock(&tlock1);
            pthread_mutex_lock(&tlock3);

            // insert function defined below
            // used to save each line in a new node of linked list
            insert(buf);
            // print function as reading the line user inputted
            // increment log index after reading each line
            printf("Logindex %d, thread 1, PID %ld, %s:  Reading stdin lines and saving into new node in linked list: %s\n",
                   logindex++, me, totalTimeAndDate, buf);
            // unlock both locks as we are done using them
            pthread_mutex_unlock(&tlock1);
            pthread_mutex_unlock(&tlock3);
        }
        // mark the boolean variable as true once we are done reading the lines and exited the while loop
        is_reading_complete = true;
    } else {
        printf("This is thread %ld and I can access the THREADDATA %p\n", me, p);
        //TODO implement any thread2 functionality.
        //Make sure to use any mutex locks appropriately

        // use helper function to get date and time for print function
        printDateAndTime();

        // set a linked list node to the existing head
        // loop through the linked list and read each node so thread 2 can output each line read from thread 1
        node *currentNode = head;
        while (!is_reading_complete) {
            if (currentNode != head) {
                // use both locks as we are incrementing log index as well as using the linked list implementation
                pthread_mutex_lock(&tlock3);

                pthread_mutex_lock(&tlock1);
                printf("Logindex %d, thread 2, PID %ld, %s:  Head of linked list contains %s\n", logindex++, me,
                       totalTimeAndDate, buf);
                currentNode = head;

                // unlock both locks after we are done using them
                pthread_mutex_unlock(&tlock1);

                pthread_mutex_unlock(&tlock3);
            }
        }
    }

    // TODO use mutex to make this a start of a critical section
    pthread_mutex_lock(&tlock2);
    pthread_mutex_lock(&tlock3);
    if (p != NULL && p->creator == me) {
        printf("This is thread %ld and I did not touch THREADDATA\n", me);
    } else {
        /**
         * TODO Free the THREADATA object.
         * Freeing should be done by the other thread from the one that created it.
         * See how the THREADDATA was created for an example of how this is done.
         */
        if (p == NULL) {
            // free the threaddata object as stated in the directions by just calling free
            // free the linked list as we don't want memory leaks
            // set the object and head to null after
            free(p);
            freeLinkedList();
            p = NULL;
            head = NULL;
            printf("This is thread %ld and I deleted the THREADDATA\n", me);
        }
    }
    // TODO critical section ends
    pthread_mutex_unlock(&tlock2);
    pthread_mutex_unlock(&tlock3);

    pthread_exit(NULL);
    return NULL;

}//end thread_runner


// helper function to free all linked list nodes once we are done using the thread
// this is done to prevent memory leaks
// set a node to the head
// loop through the linked list and traverse through the nodes, freeing each node after traversing one by one
// set the node to the head again and keep continuing the process till the linked list is null
void freeLinkedList() {
    node *currentNode;
    currentNode = head;
    while (currentNode != NULL) {
        head = head->next;
        free(currentNode);
        currentNode = head;
    }
}


// used to save the stdin lines to nodes in a linked list
// allocate a new node space using malloc
// copy the input line to the new node created
// if the linked list is null set the newNode as the head in the linked list and set its next as null
// if linked list is not null set the newNode as head and set the head node as the newNode's next
// then reassign the node head to be the head
void insert(char *buf) {
    node *newNode = (node *) malloc(sizeof(node));
    strcpy(newNode->string, buf);
    if (head == NULL) {
        head = newNode;
        head->next = NULL;
    } else {
        newNode->next = head;
        head = newNode;
    }
}


// Print current date and time in C
// template code given used and int return value
// we decided to use char* as the return variable due to the ease of using sprintf and string concatenation
char *printDateAndTime() {
    // variable to store printed out result
    char str[50];
    // variables to store date and time components
    int hours, minutes, seconds, day, month, year;

    // time_t is arithmetic time type
    time_t now;

    // Obtain current time
    // time() returns the current time of the system as a time_t value
    time(&now);

    // Convert to local time format and print to stdout
    printf("Today is : %s", ctime(&now));

    // localtime converts a time_t value to calendar time and
    // returns a pointer to a tm structure with its members
    // filled with the corresponding values
    struct tm *local = localtime(&now);

    hours = local->tm_hour;        // get hours since midnight (0-23)
    minutes = local->tm_min;        // get minutes passed after the hour (0-59)
    seconds = local->tm_sec;        // get seconds passed after minute (0-59)

    day = local->tm_mday;            // get day of month (1 to 31)
    month = local->tm_mon + 1;    // get month of year (0 to 11)
    year = local->tm_year + 1900;    // get year since 1900

    // print local time
    if (hours < 12)    // before midday
        sprintf(str, "Time is : %02d:%02d:%02d am\n", hours, minutes, seconds);

    else    // after midday
        sprintf(str, "Time is : %02d:%02d:%02d pm\n", hours - 12, minutes, seconds);

    // print current date
    sprintf(totalTimeAndDate, "Date is : %02d/%02d/%d\n", day, month, year);

    strcat(totalTimeAndDate, str);

    return totalTimeAndDate;
}



