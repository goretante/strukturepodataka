/*
Zadan je niz brojeva 2, 5, 7, 8, 11, 1, 4, 2, 3, 7 koji su spremljeni u cvorove binarnog stabla.
	a) Napisati funkciju insert koja dodaje element u stablo tako da se pozivima te funkcije za
	sve element zadanog niza brojeva stvori stablo kao na slici Slika 1. Funkcije vraca
	pokazivac na korijen stabla.
	b) Napisati funkciju replace koja ce svaki element stabla zamijeniti sumom elemenata u
	njegovom lijevom i desnom podstablu(tj.sumom svih potomaka prije zamjene
	vrijednosti u tim potomcima).
	c) Prepraviti program na nacin da umjesto predefiniranog cjelobrojnog polja koristenjem
	funkcije rand() generira slucajne brojeve u rasponu <10, 90>.Takoder, potrebno je
	upisati u datoteku sve brojeve u inorder prolasku nakon koristenja funkcije iz a), zatim b)
	dijela zadatka.
*/

#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// structures
struct _node;
typedef struct _node* Position;
typedef struct _node {
	int element;
	Position left;
	Position right;
} Node;

// prototype functions
Position createNode(int value);
Position insert(Position root, int value);
int levelOrder(Position root);
int replace(Position root);
int random();

// main
int main() {
	
	Position root = NULL;
	Position randRoot = NULL;
	srand(time(0));

	root = insert(root, 2);
	insert(root, 5);
	insert(root, 7);
	insert(root, 8);
	insert(root, 11);
	insert(root, 1);
	insert(root, 4);
	insert(root, 2);
	insert(root, 3);
	insert(root, 7);

	levelOrder(root);

	replace(root);
	printf("\n");
	levelOrder(root);

	randRoot = insert(randRoot, random());
	for (int i = 0; i < 10; i++) {
		insert(randRoot, random());
	}

	printf("\n");
	levelOrder(randRoot);

	return 0;
}

Position createNode(int value) {
	Position newNode = NULL;
	newNode = (Position)malloc(sizeof(Node));
	if (!newNode) {
		printf("Can't allocate memory!\n");
		return NULL;
	}

	newNode->element = value;
	newNode->left = NULL;
	newNode->right = NULL;
	return newNode;
}

Position insert(Position root, int value) {

	if (root == NULL) {
		return createNode(value);
	}

	if (value < root->element) root->left = insert(root->left, value);
	else if (value >= root->element) root->right = insert(root->right, value);

	return root;
}

int levelOrder(Position root) {
	if (root == NULL)
		return;

	Position queue[100] = { 0 };
	int front = 0, rear = 0;

	queue[rear++] = root;

	while (front < rear) {
		Position current = queue[front++];

		printf("%d ", current->element);

		if (current->left != NULL)
			queue[rear++] = current->left;

		if (current->right != NULL)
			queue[rear++] = current->right;
	}
	return EXIT_SUCCESS;
}

int replace(Position root) {
	if (root == NULL) {
		return 0;
	}
	int	leftElement = replace(root->left);
	int rightElement = replace(root->right);
	int originalElement = root->element;

	root->element = leftElement + rightElement;
	return originalElement + root->element;
}

int random() {
	int value = 0;
	value = rand() % 81 + 10;
	return value;
}