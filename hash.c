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

int hashGetNewAddress(int iteratedValue, int iteratedAddress) {
    int newAddress;
    int address = (iteratedValue* iteratedAddress) % HASHTABLE_SIZE + 1;

    return address;
}

int hashGetAddress(char *value){
    int address = INIT_ADDRESS;
    int node;
    for (node = 0; node < strlen(value); ++node) {
        // TODO: Verify if key already exists.
        address = hashGetNewAddress(address, node);
    }

    return address - 1;
}

HASHTABLE_NODE* hashInsert(char *value, int type){
    HASHTABLE_NODE* newHashtableNode;
    int addressToAddNode;
    addressToAddNode = hashGetAddress(value);
    newHashtableNode = (HASHTABLE_NODE*) calloc(DEFAULT_N_OBJECTS, sizeof(HASHTABLE_NODE*));
    // needed allocation to avoid a missed pointer
    newHashtableNode->value = (char*) calloc(strlen(value)+1, sizeof(char));
    newHashtableNode->type = type;
    // new node points to the node the last node was pointing to
    newHashtableNode->next = Hashtable[addressToAddNode];
    strcpy(newHashtableNode->value, value);
    // the last node now has to point to the inserted node
    Hashtable[addressToAddNode] = newHashtableNode;

    return newHashtableNode;
}

HASHTABLE_NODE* hashFind(char *value){
    HASHTABLE_NODE* hashtableNode;
    int addressNode;
    addressNode = hashGetAddress(value);
    for(hashtableNode = Hashtable[addressNode]; hashtableNode; hashtableNode = hashtableNode->next) {
        if(!strcmp(value, hashtableNode->value)) {
            return hashtableNode;
        }
    }

    return (HASHTABLE_NODE*) NODE_NOT_FOUND;
}

void hashPrint(void){
    HASHTABLE_NODE* hashtableNode;
    int node;
    for (node = INIT_VALUE; node < HASH_SIZE; ++node){
        for (hashtableNode = Hashtable[node]; hashtableNode; hashtableNode = hashtableNode->next) {
            fprintf(stderr, "Key: %d\nValue: %s\n#################\n", node, hashtableNode->value);
        }
    }
}
