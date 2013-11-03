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

Tree ins(Tree t, int i) {
	Tree T = t;
	Node** prv = NULL;
	while (t != NULL) {
		if (i < t->data) {
			prv = &(t->left);
			t = t->left;
		}
		else if (i > t->data) {
			prv = &(t->right);
			t = t->right;
		}
	}
	*prv = (Node*) malloc(sizeof(Node));
	t = *prv;
	t->data = i;
	t->right = t->left = NULL;
	return T;
}

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
	string b0s;
		while (L--) {

		}

	return 0;
}
