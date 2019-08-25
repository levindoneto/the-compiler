#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "hash.h"

HASH_NODE* HashTable[HASHTABLE_SIZE];

void hashInitialize (void){
    int node;
    for (node = 0; node < HASHTABLE_SIZE; ++node) {
        HashTable[node] = INIT_VALUE;
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

HASH_NODE* hashInsert(int type, char *text){
    // TODO
}
HASH_NODE* hashFind(char *text){
    // TODO
}

void hashPrint(void){
    // TODO
}
