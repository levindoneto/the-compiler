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

int hashGetAddress(char *text){
    // TODO;
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
