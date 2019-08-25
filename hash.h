#define INIT_VALUE 0
#define INIT_ADDRESS 1
#define DEFAULT_N_OBJECTS 1 // default number of objects to alloc memory
#define TEXT_SIZE 128
#define HASHTABLE_SIZE 997 // prime number

typedef struct HASHTABLE_NODE {
    char *text;
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
 * @parameter:  {string} text to get the ascii from the char within the string.
 * @return:     {integer} calculated address.
 */
int hashGetNewAddress(int iteratedAddress, char *text);

/* Function for getting an address
 * for placing a text into the hash table.
 * @parameter:  {string} text.
 * @return:     {integer} address.
 */
int hashGetAddress(char *text);

/* Function for inserting a text into the hash table.
 * @parameter:  {integer} type.
 * @parameter:  {string} text.
 * @return:     {HASHTABLE_NODE} node of the hash table.
 */
HASHTABLE_NODE* hashInsert(int type, char *text);

/* Function for finding the node where a text key is placed in.
 * @parameter:  {string} text.
 * @return:     {HASHTABLE_NODE} found node of the hash table.
 */
HASHTABLE_NODE* hashFind(char *text);

/* Function for printing the whole hash table in the console.
 * @parameter:  {void}.
 * @return:     {void}.
 */
void hashPrint(void);
