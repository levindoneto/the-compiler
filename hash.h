#define NODE_NOT_FOUND -1 // when the node is not found withing the hashtable
#define INIT_VALUE 0
#define INIT_ADDRESS 1
#define DEFAULT_N_OBJECTS 1 // default number of objects to alloc memory
#define TEXT_SIZE 128
#define HASHTABLE_SIZE 997 // prime number

typedef struct HASHTABLE_NODE {
    char *value;
    int type;
    struct HASHTABLE_NODE *next;
} HASHTABLE_NODE;

/* Function for initializing the hash table.
 * @parameter:  {void}.
 * @return:     {void}.
 */
void hashInitialize(void);

/* Function for obtaining the address to place the node into the hashtable.
 * @parameter:  {integer} iterated address from the for loop to get address.
 * @parameter:  {integer} value from the string from hashGetAddress.
 * @return:     {integer} calculated address.
 */
int hashGetNewAddress(int iteratedAddress, int iteratedValue);

/* Function for getting an address
 * for placing a text into the hash table.
 * @parameter:  {string} value.
 * @return:     {integer} address.
 */
int hashGetAddress(char *value);

/* Function for inserting a text into the hash table.
 * @parameter:  {string} value.
 * @parameter:  {integer} type.
 * @return:     {HASHTABLE_NODE} node of the hash table.
 */
HASHTABLE_NODE* hashInsert(char *value, int type);

/* Function for finding the node where a text value is placed in.
 * @parameter:  {string} value.
 * @return:     {HASHTABLE_NODE} found node of the hash table.
 */
HASHTABLE_NODE* hashFind(char *value);

/* Function for printing the whole hash table in the console.
 * @parameter:  {void}.
 * @return:     {void}.
 */
void hashPrint(void);
