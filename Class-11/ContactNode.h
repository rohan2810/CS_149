#ifndef CONTACT_MODE_H
#define CONTACT_MODE_H

typedef struct ContactNode_struct {
    char contactName[50];
    char contactPhoneNum[50];
    struct ContactNode *nextNodePtr;
} ContactNode;

void CreateContactNode(ContactNode *thisNode, char nameInit[], char phoneNumInit[], ContactNode *nextLoc);

void InsertContactAfter(ContactNode *thisNode, ContactNode *newNode);

ContactNode *GetNextContact();

void PrintContactNode();

#endif