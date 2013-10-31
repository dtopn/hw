#include <iostream>
#include <cstring>
#include <cstdlib>
#define MAXN 1000;
using namespace std;

int hash(int i, int n, int* table);
int ins(int i, int pos, int* table);
int* add(int i, int* heap, int size);
int main() {
	int N, n;
	cin >> N;
	int *result, *heap;
	result = (int *) malloc (n * sizeof(int));
	heap = (int *) malloc (n * sizeof(int));
	memset(result, -1, n* sizeof(int));
	memset(heap, -1, n* sizeof(int));
	int temp;
	for (int i = 0; i < N; i++) {
		cout << *(result + i);		
	}
	for (int i = 0; i < N; i++) {
		cin >> temp;
		*(result + i) = temp;
	}
	return 0;
}

int hash(int i, int n, int* table) {
	int pos;
	pos = i % n;
	while (*(table + pos) != -1) {
		if (pos == n) {
			pos = 0;
		}
		else {
			pos++;
		}
	}

	return pos;
}

int ins(int i, int pos, int* table) {
	*(table + pos) = i;
	return 0;
}

int *add(int i, int* heap, int size) {

	
	return heap;
}
