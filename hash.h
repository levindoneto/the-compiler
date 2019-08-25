#define INIT_VALUE 0
#define TEXT_SIZE 128
#define HASHTABLE_SIZE 997 // prime number

typedef struct hash_node {
    char *text;
    int type;
    struct hash_node *next;
} HASH_NODE;

/* Function for initializing the hash table.
 * @parameter:  {void}.
 * @return:     {void}.
 */
void hashInitialize(void);

/* Function for getting an address
 * for placing a text into the hash table.
 * @parameter:  {string} text.
 * @return:     {integer} address.
 */
int hashGetAddress(char *text);

/* Function for inserting a text into the hash table.
 * @parameter:  {integer} type.
 * @parameter:  {string} text.
 * @return:     {HASH_NODE} node of the hash table.
 */
HASH_NODE* hashInsert(int type, char *text);

/* Function for finding the node where a text key is placed in.
 * @parameter:  {string} text.
 * @return:     {HASH_NODE} found node of the hash table.
 */
HASH_NODE* hashFind(char *text);

/* Function for printing the whole hash table in the console.
 * @parameter:  {void}.
 * @return:     {void}.
 */
void hashPrint(void);
