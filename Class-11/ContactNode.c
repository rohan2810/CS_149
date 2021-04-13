#include <stdio.h>
#include <string.h>
#include "ContactNode.h"

void CreateContactNode(ContactNode *thisNode, char nameInit[], char phoneNumInit[], ContactNode *nextLoc) {
    strcpy(thisNode->contactName, nameInit);
    strcpy(thisNode->contactPhoneNum, phoneNumInit);
    thisNode->nextNodePtr = nextLoc;
}

void InsertContactAfter(ContactNode *thisNode, ContactNode *newNode) {
    ContactNode *temp = NULL;
    temp = thisNode->nextNodePtr;
    thisNode->nextNodePtr = newNode;
    newNode->nextNodePtr = temp;
}

ContactNode *GetNextContact(ContactNode *contactNode) {
    return contactNode->nextNodePtr;
}

void PrintContactNode(ContactNode *contactNode) {
    printf("Name: %s", contactNode->contactName);
    printf("\nPhone number: %s\n", contactNode->contactPhoneNum);
}

