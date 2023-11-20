#include <stdio.h>
#include <stdlib.h>

#define SEGMENT_SIZE 100
#define DIRECTORY_SIZE 1000
#define PRIME 1048583

typedef struct Element{
	int key;

	struct Element* next;
} Element;

typedef Element* ElementPtr;

typedef ElementPtr Segment[SEGMENT_SIZE];
typedef Segment* SegmentPtr;

typedef struct HashTable{
	int p;
	int maxp;
	int keyCount;
	int currentSize;
	float minLoadFactor;
	float maxLoadFactor;
	SegmentPtr directory[DIRECTORY_SIZE];
} HashTable;

int convertKey(int key){
	return key;
}

int hash(int K, HashTable* T){
	int h = convertKey(K) % PRIME;
	int address = h % T->maxp;

	if(address < T->p){
		address = h % (2 * T->maxp);
	}

	return address;
}

void expandTable(HashTable* T){
	int newAddress, oldSegmentIndex, newSegmentIndex;
	SegmentPtr oldSegment, newSegment;
	ElementPtr current, previous, lastOfNew;

	if(T->maxp + T->p < DIRECTORY_SIZE * SEGMENT_SIZE){
		oldSegment = T->directory[T->p / SEGMENT_SIZE];
		oldSegmentIndex = T->p % SEGMENT_SIZE;

		newAddress = T->maxp + T->p;

		if(newAddress % SEGMENT_SIZE == 0){
			T->directory[newAddress / SEGMENT_SIZE] = (SegmentPtr)malloc(sizeof(Segment));
		}

		newSegment = T->directory[newAddress / SEGMENT_SIZE];
		newSegmentIndex = newAddress % SEGMENT_SIZE;

		T->p++;

		if(T->p == T->maxp){
			T->maxp *= 2;
			T->p = 0;
		}

		T->currentSize++;

		current = oldSegment[oldSegmentIndex];
		previous = NULL;
		lastOfNew = NULL;
		newAddress
	}
}

int main(){
	printf("Hello\n");
	return 0;
}
