#include <iostream>
#include <cstdlib>
#include <string>

using namespace std;


typedef struct node {
	node* left;
	node* right;
	int data;
} Node;
typedef Node* Tree;

int main() {
	int N, L;
	cin >> N >> L;
	int n = N;
	int temp;
	Tree b0 = (Tree) malloc(sizeof (Node));
	b0->left = NULL; b0->right = NULL;
	cin >> temp;
	b0->data = temp;
	while (--n) {
		cin >> temp;
		b0 = ins(b0, temp);
	}
	string 
	while (L--) {
			
	}

	return 0;
}
