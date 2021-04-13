#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#include "ContactNode.h"

int main(void) {

    ContactNode *contactNode1 = (ContactNode *) malloc(sizeof(ContactNode));
    ContactNode *contactNode2 = (ContactNode *) malloc(sizeof(ContactNode));
    ContactNode *contactNode3 = (ContactNode *) malloc(sizeof(ContactNode));
    char contactName[50];
    char contactPhoneNum[50];

    printf("Person 1\n");
    printf("Enter name:\n");
    fgets(contactName, 50, stdin);
    printf("Enter phone number:\n");
    fgets(contactPhoneNum, 50, stdin);
    contactName[strlen(contactName) - 1] = '\0';
    contactPhoneNum[strlen(contactPhoneNum) - 1] = '\0';
    printf("You entered: %s, %s", contactName, contactPhoneNum);
    printf("\n\n");
    CreateContactNode(contactNode1, contactName, contactPhoneNum, contactNode2);
    InsertContactAfter(contactNode1, contactNode2);

    printf("Person 2\n");
    printf("Enter name:\n");
    fgets(contactName, 50, stdin);
    printf("Enter phone number:\n");
    fgets(contactPhoneNum, 50, stdin);
    contactName[strlen(contactName) - 1] = '\0';
    contactPhoneNum[strlen(contactPhoneNum) - 1] = '\0';
    printf("You entered: %s, %s", contactName, contactPhoneNum);
    printf("\n\n");
    CreateContactNode(contactNode2, contactName, contactPhoneNum, contactNode3);
    InsertContactAfter(contactNode2, contactNode3);

    printf("Person 3\n");
    printf("Enter name:\n");
    fgets(contactName, 50, stdin);
    printf("Enter phone number:\n");
    fgets(contactPhoneNum, 50, stdin);
    contactName[strlen(contactName) - 1] = '\0';
    contactPhoneNum[strlen(contactPhoneNum)] = '\0';
    printf("You entered: %s, %s", contactName, contactPhoneNum);
    printf("\n\n");
    CreateContactNode(contactNode3, contactName, contactPhoneNum, NULL);

    printf("CONTACT LIST\n");
    ContactNode *thisNode = contactNode1;
    while (thisNode != NULL) {
        PrintContactNode(thisNode);
        printf("\n");
        thisNode = GetNextContact(thisNode);
    }

    return 0;
}