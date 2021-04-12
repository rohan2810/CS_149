#include <stdio.h>
#include "ContactNode.h"
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

// char contactName[50];
//    char contactPhoneNum[50];
//    struct ContactNode* nextNodePtr;


void CreateContactNode(ContactNode *thisNode, char nameInit[], char phoneNumInit[], ContactNode *nextLoc) {
    strcpy(thisNode->contactName, nameInit);
    strcpy(thisNode->contactPhoneNum, phoneNumInit);
    thisNode->nextNodePtr = (struct ContactNode *) nextLoc;
}

void InsertContactAfter(ContactNode *thisNode, ContactNode *newNode) {
    thisNode->nextNodePtr = (struct ContactNode *) newNode;
}

ContactNode *GetNextContact(ContactNode contactNode) {
    return (ContactNode *) contactNode.nextNodePtr;
}

void PrintContactNode(ContactNode contactNode) {
    printf("Name\n: %s", contactNode.contactName);
    printf("Phone Number: %s", contactNode.contactPhoneNum);
}

