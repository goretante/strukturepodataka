#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

struct _node;
typedef struct _node* Position;

typedef struct _node {
    int element;
    Position left;
    Position right;
} Node;

Position createNode(int value);
Position insert(Position root, int value);
void inorderToFile(Position root, FILE* filePointer);
void inorderToConsole(Position root);
int replace(Position root);
int getRandomValue();

int main() {
    Position root = NULL;
    Position rootRand = NULL;
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

    inorderToConsole(root);
    printf("\n");

    replace(root);
    inorderToConsole(root);
    printf("\n");

    rootRand = insert(rootRand, getRandomValue());
    for (int i = 0; i < 10; i++) {
        insert(rootRand, getRandomValue());
    }
    FILE* filePointer = fopen("inorder.txt", "w");
    inorderToFile(rootRand, filePointer);
    replace(rootRand);

    fclose(filePointer);
    return 0;
}

Position createNode(int value) {
    Position newNode = (Position)malloc(sizeof(Node));
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

    if (value >= root->element)
        root->left = insert(root->left, value);
    else if (value < root->element)
        root->right = insert(root->right, value);

    return root;
}

void inorderToFile(Position root, FILE* filePointer) {
    if (root != NULL) {
        inorderToFile(root->left, filePointer);
        fprintf(filePointer, "%d ", root->element);
        inorderToFile(root->right, filePointer);
    }
}

void inorderToConsole(Position root) {
    if (root != NULL) {
        inorderToConsole(root->left);
        printf("%d ", root->element);
        inorderToConsole(root->right);
    }
}

int replace(Position root) {
    int leftElement = 0;
    int rightElement = 0;
    int originalElement = 0;

    if (root == NULL) {
        return 0;
    }

    leftElement = replace(root->left);
    rightElement = replace(root->right);
    originalElement = root->element;

    root->element = leftElement + rightElement;
    return originalElement + root->element;
}

int getRandomValue() {
    return rand() % 81 + 10;
}
