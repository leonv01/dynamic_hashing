#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <CUnit/Basic.h>

int elements = 1000000;

typedef struct Node{
	char* key;
	int value;
	struct Node* left;
	struct Node* right;
} Node;

typedef struct BinarySearchTree{
	Node* root;
} BinarySearchTree;

BinarySearchTree* createBinarySearchTree(){
	BinarySearchTree* tree = malloc(sizeof(BinarySearchTree));
	tree->root = NULL;
	return tree;
}

Node* createNode(char* key, int value){
	Node* node = malloc(sizeof(Node));
	node->key = key;
	node->value = value;
	node->left = NULL;
	node->right = NULL;
	return node;
}

void insert(BinarySearchTree* tree, char* key, int value){
	Node* node = createNode(key, value);
	if(tree->root == NULL){
		tree->root = node;
		return;
	}
	Node* current = tree->root;
	while(1){
		if(strcmp(key, current->key) < 0){
			if(current->left == NULL){
				current->left = node;
				return;
			}
			current = current->left;
		}else{
			if(current->right == NULL){
				current->right = node;
				return;
			}
			current = current->right;
		}
	}
}

char* generateRandomString(int length){
	char* string = malloc((length + 1) * sizeof(char));
	char charset[] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";

	for(int i = 0; i < length; i++){
		int index = rand() % (sizeof(charset) - 1);
		string[i] = charset[index];
	}

	string[length] = '\0';

	return string;
}

void measureTime(BinarySearchTree* tree, char* key, int value){
	clock_t start, end;
	double cpu_time_used;

	start = clock();
	insert(tree, key, value);
	end = clock();

	cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
	printf("Time taken: %.12f\n", cpu_time_used);
}

void freeNode(Node* node){
	if(node->left != NULL){
		freeNode(node->left);
	}
	if(node->right != NULL){
		freeNode(node->right);
	}
	free(node);
}

void freeBinarySearchTree(BinarySearchTree* tree){
	if(tree->root == NULL){
		free(tree);
		return;
	}
	freeNode(tree->root);
}

void test_measureTime(void){
    BinarySearchTree* tree = createBinarySearchTree();

    srand(1); // seed the random number generator

    clock_t start = clock();
    for(int i = 0; i < elements; i++){
        char key[20];
        sprintf(key, "testKey%d", i);
        int value = rand(); // generate a random value
        measureTime(tree, key, value);
    }
    clock_t end = clock();

    double time_taken = ((double) end - start) / CLOCKS_PER_SEC;

    CU_ASSERT(time_taken < 10.0);

    freeBinarySearchTree(tree);

    printf("Elements: %d\n", elements);
}

int main(int argc, char** argv){
	if(argc > 1){
		elements = atoi(argv[1]);
	}

    if (CUE_SUCCESS != CU_initialize_registry())
        return CU_get_error();

    CU_pSuite suite = CU_add_suite("binary_search_tree", 0, 0);
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