#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>


struct _node;
typedef struct _node* Position;
typedef struct _node {
	int value;
	Position left;
	Position right;
} Node;

Position insert(Position root, int value);
Position createNode(int value);
int inOrder(Position root);
int preOrder(Position root);
int postOrder(Position root);
int levelOrder(Position root);
Position find(Position root, int value);
Position delete(Position root, int value);

int main() {

	Position root = NULL;
	
	root = insert(root, 8);
	insert(root, 3);
	insert(root, 2);
	insert(root, 1);
	insert(root, 9);
	insert(root, 7);

	printf("in order: ");
	inOrder(root);
	printf("\npre order: ");
	preOrder(root);
	printf("\npost order: ");
	postOrder(root);
	printf("\nlevel order: ");
	levelOrder(root);
	printf("\n");

	int findVal;
	printf("Find number: ");
	scanf("%d", &findVal);
	Position result = find(root, findVal);
	if (result) printf("Element %d is on address %p\n", findVal, result);
	else printf("Can't find the element.\n");

	int deleteVal;
	printf("Delete number: ");
	scanf("%d", &deleteVal);
	delete(root, deleteVal);

	return EXIT_SUCCESS;
}

int inOrder(Position root) {
	if (root != NULL) {

		inOrder(root->left);
		printf("%d ", root->value);
		inOrder(root->right);
	}
	return EXIT_SUCCESS;
}

int preOrder(Position root) {
	if (root != NULL) {
		printf("%d ", root->value);
		preOrder(root->left);
		preOrder(root->right);
	}

	return EXIT_SUCCESS;
}

int postOrder(Position root) {
	if (root != NULL) {
		postOrder(root->left);
		postOrder(root->right);
		printf("%d ", root->value);
	}
}

int levelOrder(Position root) {
	if (root == NULL)
		return;

	Position stack[100] = { 0 };
	int first = 0;
	int last = 0;

	stack[last++] = root;

	while (first < last) {
		Position current = stack[first++];

		printf("%d ", current->value);

		if (current->left != NULL)
			stack[last++] = current->left;
		if (current->right != NULL)
			stack[last++] = current->right;
	}

	return EXIT_SUCCESS;
}

Position insert(Position root, int value) {
	if (root == NULL) {
		return createNode(value);
	}
	if (value < root->value) {
		root->left = insert(root->left, value);
	} else if (value >= root->value) {
		root->right = insert(root->right, value);
	}

	return root;
}

Position createNode(int value) {

	Position newNode = NULL;
	newNode = (Position)malloc(sizeof(Node));
	if (!newNode) {
		printf("Memory can't be allocated.\n");
		return NULL;
	}
	newNode->value = value;
	newNode->left = NULL;
	newNode->right = NULL;
	return newNode;
}

Position find(Position root, int value) {

	if (root == NULL) {
		return NULL;
	}

	else if (value < root->value) {
		return find(root->left, value);
	}

	else if (value > root->value) {
		return find(root->right, value);
	}

	return root;

}

Position delete(Position root, int value) {
	Position temp = NULL;

	if (!root) {
		printf("Element kojeg zelite obrisati nije u stablu.\n");
	}

	else if (value < root->value)
		root->left = delete(root->left, value);

	else if (value > root->value)
		root->right = delete(root->right, value);


	else if (root->left != NULL && root->right != NULL) {

		temp = root->right;

		while (temp->left != NULL)
			temp = temp->left;

		root->value = temp->value;

		root->right = delete(root->right, temp->value);

	}

	else {

		temp = root;

		if (root->left != NULL)
			root = root->left;

		else
			root = root->right;

		free(temp);

	}

	return root;

}