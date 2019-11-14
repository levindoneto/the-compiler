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
        address = hashGetNewAddress(address, value[node]);
    }

    return address - 1;
}

HASHTABLE_NODE* hashInsert(char *value, int type){
    HASHTABLE_NODE* newHashtableNode;
    int addressToAddNode;
    addressToAddNode = hashGetAddress(value);
    if (hashFind(value) == NULL) { // it just adds to the hashtable if the value hasn't been found before
        newHashtableNode = (HASHTABLE_NODE*) calloc(DEFAULT_N_OBJECTS, sizeof(HASHTABLE_NODE));
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

    return hashFind(value);
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

    return NULL;
}

int hashGetNumberUndeclared(void){
    HASHTABLE_NODE* hashtableNode;
    int numberSemanticErrors = INIT_VALUE;
    int node;

    for(node = 0; node < HASHTABLE_SIZE; ++node) {
        for(hashtableNode = Hashtable[node]; hashtableNode; hashtableNode = hashtableNode->next){
            if(hashtableNode->type == SYMBOL_IDENTIFIER){
                fprintf(stderr, "Semantic ERROR: Symbol %s undeclared.\n", hashtableNode->value);
                numberSemanticErrors++;
            }
        }
    }
    return numberSemanticErrors;
}

void hashPrint(void){
    HASHTABLE_NODE* hashtableNode;
    int node;
    printf("\nHASHTABLE:\n");
    for (node = INIT_VALUE; node < HASHTABLE_SIZE; ++node){
        for (hashtableNode = Hashtable[node]; hashtableNode; hashtableNode = hashtableNode->next) {
            fprintf(stderr, "'%d': {'value': %s, 'type': %d},\n", node, hashtableNode->value, hashtableNode->type);
        }
    }
}

HASHTABLE_NODE* makeLabel() {
    static char name[MAX_NAME];
    static int serial = INIT_VALUE;
    sprintf(name, "Label%d", serial++);
    return hashInsert(name, SYMBOL_LABEL);
}

HASHTABLE_NODE* makeTemp() {
    static char name[MAX_NAME];
    static int serial = INIT_VALUE;
    sprintf(name, "Temp%d", serial++);
    return hashInsert(name, SYMBOL_SCALAR);
}
