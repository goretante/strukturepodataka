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

struct _queueNode;
typedef struct _queueNode* QueueNode;
typedef struct _queueNode {
    Position treeNode;
    QueueNode next;
} QueueNodeStruct;

typedef struct {
    QueueNode front;
    QueueNode rear;
} Queue;

Position insert(Position root, int value);
Position createNode(int value);
int inOrder(Position root);
int preOrder(Position root);
int postOrder(Position root);
int levelOrder(Position root);
Position find(Position root, int value);
Position delete(Position root, int value);
Queue* createQueue();
void enqueue(Queue* queue, Position treeNode);
QueueNode dequeueNode(Queue* queue);
int isQueueEmpty(Queue* queue);

int main() {

    Position root = NULL;
    int choice, value;

    do {
        printf("\n1. Add element\n");
        printf("2. Print inorder\n");
        printf("3. Print preorder\n");
        printf("4. Print postorder\n");
        printf("5. Print levelorder\n");
        printf("6. Find element\n");
        printf("7. Delete element\n");
        printf("8. Exit\n");

        printf("\nChoose option: ");
        scanf("%d", &choice);

        switch (choice) {
        case 1:
            printf("Enter value: ");
            scanf("%d", &value);
            root = insert(root, value);
            break;
        case 2:
            printf("Inorder: ");
            inOrder(root);
            break;
        case 3:
            printf("Preorder: ");
            preOrder(root);
            break;
        case 4:
            printf("Postorder: ");
            postOrder(root);
            break;
        case 5:
            printf("Levelorder: ");
            levelOrder(root);
            break;
        case 6:
            printf("Enter value you want to find: ");
            scanf("%d", &value);
            Position result = find(root, value);
            if (result)
                printf("Element %d is on address %p\n", value, result);
            else
                printf("Can't find the element.\n");
            break;
        case 7:
            printf("Enter value you want to delete: ");
            scanf("%d", &value);
            root = delete(root, value);
            break;
        case 8:
            printf("Program exit.\n");
            break;
        default:
            printf("Invalid choice, try again.\n");
        }
    } while (choice != 8);

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
    return EXIT_SUCCESS;
}

int levelOrder(Position root) {
    if (root == NULL)
        return EXIT_SUCCESS;

    Queue* queue = createQueue();
    enqueue(queue, root);

    while (!isQueueEmpty(queue)) {
        Position current = dequeueNode(queue)->treeNode;

        printf("%d ", current->value);

        if (current->left != NULL)
            enqueue(queue, current->left);
        if (current->right != NULL)
            enqueue(queue, current->right);
    }

    return EXIT_SUCCESS;
}

Position insert(Position root, int value) {
    if (root == NULL) {
        return createNode(value);
    }
    if (value < root->value) {
        root->left = insert(root->left, value);
    }
    else if (value > root->value) {
        root->right = insert(root->right, value);
    }

    return root;
}

Position createNode(int value) {

    Position newNode = NULL;
    newNode = (Position)malloc(sizeof(Node));
    if (!newNode) {
        fprintf(stderr, "Can't allocate memory.\n");
        exit(EXIT_FAILURE);
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
    Position successor = NULL;

    if (!root) {
        printf("Element you want to delete is not in the tree.\n");
    }
    else if (value < root->value)
        root->left = delete(root->left, value);
    else if (value > root->value)
        root->right = delete(root->right, value);
    else if (root->left != NULL && root->right != NULL) {
        successor = root->right;
        while (successor->left != NULL)
            successor = successor->left;
        root->value = successor->value;
        root->right = delete(root->right, successor->value);
    }
    else {
        successor = root;
        if (root->left != NULL)
            root = root->left;
        else
            root = root->right;
        free(successor);
    }

    return root;
}

Queue* createQueue() {
    Queue* queue = (Queue*)malloc(sizeof(Queue));
    if (!queue) {
        fprintf(stderr, "Can't allocate memory.\n");
        exit(EXIT_FAILURE);
    }
    queue->front = NULL;
    queue->rear = NULL;
    return queue;
}

void enqueue(Queue* queue, Position treeNode) {
    QueueNode newQueueNode = (QueueNode)malloc(sizeof(QueueNodeStruct));
    if (!newQueueNode) {
        fprintf(stderr, "Can't allocate memory.\n");
        exit(EXIT_FAILURE);
    }
    newQueueNode->treeNode = treeNode;
    newQueueNode->next = NULL;

    if (queue->rear == NULL) {
        queue->front = newQueueNode;
        queue->rear = newQueueNode;
    }
    else {
        queue->rear->next = newQueueNode;
        queue->rear = newQueueNode;
    }
}

QueueNode dequeueNode(Queue* queue) {
    if (isQueueEmpty(queue)) {
        fprintf(stderr, "Queue is empty.\n");
        exit(EXIT_FAILURE);
    }

    QueueNode frontNode = queue->front;
    queue->front = frontNode->next;

    if (queue->front == NULL) {
        queue->rear = NULL;
    }

    return frontNode;
}

int isQueueEmpty(Queue* queue) {
    return (queue->front == NULL);
}