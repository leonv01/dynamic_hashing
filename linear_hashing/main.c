#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <CUnit/Basic.h>

#define BUCKET_SIZE 100

/**
 * @struct Node
 * Represents a node in a data structure.
 */
typedef struct Node{
	char* key;
	int value;
	struct Node* next;
} Node;

/**
 * @struct Bucket
 * Represents a bucket in linear hashing.
 */
typedef struct Bucket{
	Node* head;
	int count;
} Bucket;

/**
 * @struct HashTable
 * @brief Represents a hash table data structure.
 */
typedef struct HashTable{
	Bucket** buckets;
	int size;
	int p;
	int maxp;
} HashTable;

void split(HashTable* table);
int hash(char* key);

/**
 * Creates a new HashTable with the specified size.
 *
 * @param size The size of the HashTable.
 * @return A pointer to the newly created HashTable.
 */
HashTable* createHashTable(int size) {
    HashTable* table = malloc(sizeof(HashTable));
    table->buckets = malloc(sizeof(Bucket*) * size);
    for (int i = 0; i < size; i++) {
        table->buckets[i] = malloc(sizeof(Bucket));
        table->buckets[i]->head = NULL;
        table->buckets[i]->count = 0;
    }
    table->size = size;
    table->p = 0;
    table->maxp = size;
    return table;
}


/**
 * Creates a new node with the given key and value.
 *
 * @param key The key of the node.
 * @param value The value of the node.
 * @return A pointer to the newly created node.
 */
Node* createNode(char* key, int value){
	Node* node = malloc(sizeof(Node));
	node->key = key;
	node->value = value;
	node->next = NULL;
	return node;
}

/**
 * Inserts a key-value pair into the hash table.
 *
 * @param table The hash table to insert into.
 * @param key The key to insert.
 * @param value The value to insert.
 */
void insert(HashTable* table, char* key, int value) {
    int index = hash(key) % table->maxp;
    if (index < table->p) {
        index = hash(key) % (2 * table->maxp);
    }
    Node* newNode = createNode(key, value);
    newNode->next = table->buckets[index]->head;
    table->buckets[index]->head = newNode;
    table->buckets[index]->count++;
    if (table->buckets[index]->count > BUCKET_SIZE) {
        split(table);
    }
}

/**
 * Splits the given hash table.
 *
 * @param table The hash table to be split.
 */
void split(HashTable* table){
    int oldIndex = table->p;
    int newIndex = table->p + table->maxp;
    
    if(newIndex >= table->size){
        table->buckets = realloc(table->buckets, sizeof(Bucket) * table->size * 2);
        for(int i = table->size; i < table->size * 2; i++){
            table->buckets[i] = (Bucket*)malloc(sizeof(Bucket));

            table->buckets[i]->head = NULL;
            table->buckets[i]->count = 0;
        }
        table->size *= 2;
    }

    Node* node = table->buckets[oldIndex]->head;
    Node* prev = NULL;

	while(node != NULL){
		int index = hash(node->key) % (table->maxp * 2);
		if(index == newIndex){
			if(prev == NULL){
				table->buckets[oldIndex]->head = node->next;
			}
			else{
				prev->next = node->next;
			}

			table->buckets[oldIndex]->count--;
			node->next = table->buckets[newIndex]->head;
			table->buckets[newIndex]->head = node;
			table->buckets[newIndex]->count++;
			node = (prev == NULL) ? table->buckets[oldIndex]->head : prev->next;
		}
		else{
			prev = node;
			node = node->next;	
		}
	}
	table->p++;
	if(table->p == table->maxp){
		table->p = 0;
		table->maxp *= 2;
	}
}

/**
 * @brief Computes the hash value for a given key.
 * 
 * @param key The key to be hashed.
 * @return The hash value of the key.
 */
int hash(char* key){
	int hash = 0;
	for(int i = 0; key[i] != '\0'; i++){
		hash += key[i];
	}
	return hash;
}

char* generateRandomString(int length) {
    char* string = malloc((length + 1) * sizeof(char));
    char charset[] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";

    for (int i = 0; i < length; i++) {
        int index = rand() % (sizeof(charset) - 1);
        string[i] = charset[index];
    }

    string[length] = '\0';

    return string;
}

/**
 * Measures the time taken to perform an operation on a hash table.
 *
 * @param table The hash table on which the operation is performed.
 * @param key The key used for the operation.
 * @param value The value associated with the key.
 */
void measureTime(HashTable* table, char* key, int value){
	clock_t start, end;
	double cpu_time_used;

	start = clock();
	insert(table, key, value);
	end = clock();

	cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
	printf("Time taken: %.12f\n", cpu_time_used);
}

/**
 * Frees the memory allocated for a hash table.
 *
 * @param table The hash table to be freed.
 */
void freeHashTable(HashTable* table) {
    for (int i = 0; i < table->size; i++) {
        Node* node = table->buckets[i]->head;
        while (node != NULL) {
            Node* temp = node;
            node = node->next;
            free(temp);
        }
        free(table->buckets[i]);
    }
    free(table->buckets);
    free(table);
}

/**
 * @brief This function is used to test and measure the execution time of a specific operation.
 * 
 * @details The test_measureTime function is designed to measure the execution time of a specific operation.
 * It can be used to evaluate the performance of different algorithms or functions.
 * 
 * @param void No parameters are required for this function.
 * 
 * @return void This function does not return any value.
 */
void test_measureTime(void) {
    HashTable* table = createHashTable(1);
    int value = 123;

    clock_t start = clock();
    for (int i = 0; i < 1000000; i++) { 
        char key[20];
        sprintf(key, "testKey%d", i); 
        measureTime(table, key, value);
    }
    clock_t end = clock();

    double time_taken = ((double) end - start) / CLOCKS_PER_SEC;

    CU_ASSERT(time_taken < 1.0);

	printf("Entries: %d\n", 10000000);
	printf("Size: %d\n", table->size);

    freeHashTable(table);
}

/**
 * @file main.c
 * @brief This is the main file for the linear hashing program.
 *        It contains the main function that serves as the entry point of the program.
 */
int main() {
    if (CUE_SUCCESS != CU_initialize_registry())
        return CU_get_error();

    CU_pSuite suite = CU_add_suite("hash_table_test", 0, 0);
    if (NULL == suite) {
        CU_cleanup_registry();
        return CU_get_error();
    }

    if ((NULL == CU_add_test(suite, "test of measureTime()", test_measureTime))) {
        CU_cleanup_registry();
        return CU_get_error();
    }

    CU_basic_set_mode(CU_BRM_VERBOSE);
    CU_basic_run_tests();
    CU_cleanup_registry();
    return CU_get_error();
}