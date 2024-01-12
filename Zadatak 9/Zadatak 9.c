// napomena: u c) dijelu zadatka nisam bio siguran treba li ispisivati u datoteku i binarna stabla 
// iz a) i b) dijela zadataka ili samo random brojeve koji se upisuju u rootRand, stoga sam ispisao u file
// inorder zapise iz a), b) i c) dijela zadatka 


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
void inorderToConsole(Position root); // function for testing
int replace(Position root);
int getRandomValue();

int main() {
    Position root = NULL;
    Position rootRand = NULL;
    FILE* filePointer = fopen("inorder.txt", "w");
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

    fprintf(filePointer, "a) binary tree: ");
    inorderToFile(root, filePointer);
    fprintf(filePointer, "\n");

    replace(root);
    fprintf(filePointer, "b) binary tree: ");
    inorderToFile(root, filePointer);
    fprintf(filePointer, "\n");

    rootRand = insert(rootRand, getRandomValue());
    for (int i = 0; i < 10; i++) {
        insert(rootRand, getRandomValue());
    }
    fprintf(filePointer, "c) binary tree: ");
    inorderToFile(rootRand, filePointer);

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
