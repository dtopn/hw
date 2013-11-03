#include <iostream>
#include <cstring>
#include <cstdlib>

using namespace std;

int hash(int i, int n, int* table);
int ins(int i, int pos, int* table);

int* sort(int* begin, int* end);

int main() {
	int N, n;
	cin >> N;
	n = N;
	int *result, *heap, *test;
	result = (int *) malloc (n * sizeof(int));
	heap = (int *) malloc (n * sizeof(int));
	test = (int *) malloc (n * sizeof(int));
	memset(test, -1, n * sizeof(int));
	int temp;
	for (int i = 0; i < N; i++) {
		cin >> temp;
		*(result + i) = temp;
	}
	memcpy(heap, result, n * sizeof(int));
	sort(heap, heap + n);
	//space reclaimation is ignored in this program
	int poped = 0;
	int *tmp, posi;
	tmp = heap;
	while(*heap == -1)
		heap++, poped++, tmp = heap;
	while (poped != n) {
		tmp = heap;
		if(*heap == -1)
			heap++, tmp = heap;
		else {
			while (result[posi = hash(*tmp, n, test)] != *tmp) {
				do {
					tmp++;
				}
				while (*tmp == -1);
			}
			ins(*tmp, posi, test);
			if (poped != n - 1)
			cout << *tmp << ' ';
			else
				cout << *tmp << endl;
			*tmp = -1; poped++;
		}
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

int* sort(int* begin, int* end) {
	int x;
	int *i, *j;
	for (i = begin + 1; i < end; i++) {
		x = *i;
		for (j = i; j != begin && *(j - 1) > x; j--) {
			*j = *(j - 1);
		}
		*j = x;
	}
	return begin;
}