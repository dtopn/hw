#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct node Node;
struct node {
	Node* left;
	Node* right;
	int data;
};
typedef Node* Tree;

Tree ins(Tree, int);

void parse(int* bi, Tree b) {
	Tree queue[1000];
	int head, tail;
	head = tail = 0;
	queue[head] = b;
	while (head <= tail) {
		if (queue[head]->left) queue[++tail] = queue[head]->left;
		if (queue[head]->right) queue[++tail] = queue[head]->right;
		bi[head] = queue[head]->data;
		++head;
	}
}

void compare(int *b1, int* b2, int n) {
	int i;
	int yes = 1;
	for (i = 0; i < n; i++) {
		if (b1[i] != b2[i])
			yes = 0;
	}
	if (yes) printf("Yes\n");
	else printf("No\n");
}

Tree build(Tree b0, int n) {
	int temp;
	b0 = (Tree)malloc(sizeof (Node));
	b0->left = NULL; b0->right = NULL;
	//cin >> temp;
	scanf("%d", &temp);
	b0->data = temp;
	while (--n) {
		//cin >> temp;
		scanf("%d", &temp);
		b0 = ins(b0, temp);
	}
	return b0;
}

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
	*prv = (Node*)malloc(sizeof(Node));
	t = *prv;
	t->data = i;
	t->right = t->left = NULL;
	return T;
}

int main() {
	int N, L;
	//cin >> N >> L;
	scanf("%d ", &N);
	int n = N;
	Tree b0 = NULL;
	Tree b = NULL;
	while (N) {
		scanf("%d", &L);
		b0 = build(b0, n);
		int* b0i = (int*)malloc(sizeof(int)* N);
		parse(b0i, b0);
		while (L--) {
			b = build(b, n);
			int* bi = (int*)malloc(sizeof(int)* N);
			parse(bi, b);
			compare(bi, b0i, n);
			free(bi); free(b);
		}
		free(b0i); free(b0);
		scanf("%d ", &N);
		n = N;
	}
	return 0;
}
