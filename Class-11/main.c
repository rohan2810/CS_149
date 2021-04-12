#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#include "ContactNode.h"

int main(void) {

    int NUM = 3;
    ContactNode node[NUM];
    ContactNode *contactNode1;
    ContactNode *contactNode2;
    ContactNode *contactNode3;
    char contactName[50];
    char contactPhoneNum[50];
    printf("PERSON 1");
    printf("Enter name: ");
    scanf("%s", contactName);
    printf("Enter phone number: ");
    scanf("%s", contactPhoneNum);
    printf("You entered: %s,%s", contactName, contactPhoneNum);
    CreateContactNode(contactNode1, contactName, contactPhoneNum, contactNode2);
    InsertContactAfter(contactNode1, contactNode2);
    printf("PERSON 2");
    printf("Enter name: ");
    scanf("%s", contactName);
    printf("Enter phone number: ");
    scanf("%s", contactPhoneNum);
    printf("You entered: %s,%s", contactName, contactPhoneNum);
    CreateContactNode(contactNode2, contactName, contactPhoneNum, contactNode3);
    InsertContactAfter(contactNode2, contactNode3);
    printf("PERSON 3");
    printf("Enter name: ");
    scanf("%s", contactName);
    printf("Enter phone number: ");
    scanf("%s", contactPhoneNum);
    printf("You entered: %s,%s", contactName, contactPhoneNum);
    CreateContactNode(contactNode3, contactName, contactPhoneNum, NULL);
    InsertContactAfter(contactNode3, NULL);


    return 0;
}