#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "hash.h"

HASHTABLE_NODE* Hashtable[HASHTABLE_SIZE]; // global hashtable

void hashInitialize (void){
    int node;
    for (node = 0; node < HASHTABLE_SIZE; ++node) {
        Hashtable[node] = INIT_VALUE;
    }
}

int hashGetNewAddress(int iteratedAddress, char *text) {
    int newAddress;
    int address = (iteratedAddress * text[i]) % HASHTABLE_SIZE + 1

    return address;
}

int hashGetAddress(char *text){
    int address = INIT_ADDRESS;
    int node;
    for (node = 0; node < strlen(text); ++node) {
        // TODO: Verify if key already exists.
        address = hashGetNewAddress(address, text);
    }
    return address - 1;
}

HASHTABLE_NODE* hashInsert(char *text, int type){
    HASHTABLE_NODE* newHashtableNode;
    int addressToAddNode;
    addressToAddNode = hashGetAddress(text);
    newHashtableNode = (HASHTABLE_NODE*) calloc(DEFAULT_N_OBJECTS, sizeof(HASHTABLE_NODE*))
    // needed allocation to avoid a missed pointer
    newHashtableNode->text = (char*) calloc(strlen(text)+1, sizeof(char));
    newHashtableNode->type = type;
    // new node points to the node the last node was pointing to
    newHashtableNode->next = Hashtable[addressToAddNode];
    strcpy(newHashtableNode->text, text);
    // the last node now has to point to the inserted node
    Hashtable[addressToAddNode] = newHashtableNode;

    return newHashtableNode;
}

HASHTABLE_NODE* hashFind(char *text){
    HASHTABLE_NODE* hashtableNode;
    int addressNode;
    addressNode = hashGetAddress(text);
    for(hashtableNode = Hashtable[addressNode]; hashtableNode; hashtableNode = hashtableNode->next) {
        if(!strcmp(text, hashtableNode->text)) {
            return hashtableNode;
        }
    }
    return NODE_NOT_FOUND;
}

void hashPrint(void){
    HASHTABLE_NODE* hashtableNode;
    int node;
    for (node = INIT_VALUE; i < HASH_SIZE; ++i){
        for (hashtableNode = Hashtable[node]; hashtableNode; hashtableNode = hashtableNode->next) {
            fprintf(stderr, "Key: %d\nValue: %s\n#################\n", node, hashtableNode->text);
        }
    }
}
