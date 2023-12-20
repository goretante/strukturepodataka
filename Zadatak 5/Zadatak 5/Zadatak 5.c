#define _CRT_SECURE_NO_WARNINGS

#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#define MAX_LENGHT 1024
#define ALLOC_ERROR NULL
#define FILE_ERROR -1
#define OPERATION_ERROR -2

struct _stack;
typedef struct _stack* Position;
typedef struct _stack {
	int Element;
	Position Next;
}stack;

Position initialization(Position head);
int readFromFile(char* buffer);
int calculate(Position head);
int logic(Position head, char* buffer);
int push(Position head, int numberToPush);
int pop(Position head);
int operation(Position head, char operation);
int delete(Position head);

int main()
{
	Position head = NULL;
	int result;
	head = initialization(head);
	result = calculate(head);
	if (result == OPERATION_ERROR) {
		return OPERATION_ERROR;
	}
	printf("%d\n", result);
	free(head);
	return EXIT_SUCCESS;
}

int calculate(Position head) {
	int result = 0;
	int status = 0;
	char buffer[MAX_LENGHT] = { 0 };
	readFromFile(buffer);
	status = logic(head, buffer);
	if (status == OPERATION_ERROR) {
		return OPERATION_ERROR;
	}
	result = head->Next->Element;
	head->Next = head->Next->Next;
	delete(head);
	return result;
}

int logic(Position head, char* buffer) {
	char* currentBuffer = buffer;
	int number = 0;
	int status = 0;
	int operationChar = '\0';
	int numBytes = 0;

	while (strlen(currentBuffer) > 0) {
		if (sscanf(currentBuffer, " %d %n", &number, &numBytes) == 1) {
			push(head, number);
		}
		else {
			sscanf(currentBuffer, " %c %n", &operationChar, &numBytes);
			status = operation(head, operationChar);
			if (status == OPERATION_ERROR) {
				return OPERATION_ERROR;
			}
		}

		currentBuffer += numBytes;
	}

	return EXIT_SUCCESS;
}

int operation(Position head, char operation) {

	int num1 = 0;
	int num2 = 0;

	num1 = pop(head);
	num2 = pop(head);

	switch (operation)
	{
	case '+':
		num1 = num2 + num1;
		push(head, num1);
		break;
	case '-':
		num1 = num2 - num1;
		push(head, num1);
		break;
	case '*':
		num1 = num2 * num1;
		push(head, num1);
		break;
	case '/':
		num1 = num2 / num1;
		push(head, num1);
		break;
	default:
		printf("Wrong operation!\n");
		delete(head);
		return OPERATION_ERROR;
	}
}

int delete(Position head) {
	
	Position toDelete = NULL;

	while (head->Next != NULL) {
		toDelete = head->Next;
		head->Next = toDelete->Next;
		free(toDelete);
	}

	return EXIT_SUCCESS;
}

int pop(Position head) {

	Position toDelete = head->Next;
	int numberToPop = toDelete->Element;
	head->Next = toDelete->Next;

	free(toDelete);

	return numberToPop;
}

int push(Position head, int numberToPush) {
	Position newStackElement = NULL;
	newStackElement = initialization(head);

	newStackElement->Element = numberToPush;
	newStackElement->Next = head->Next;
	head->Next = newStackElement;

	return EXIT_SUCCESS;
}

int readFromFile(char* buffer) {
	FILE* filePointer = NULL;
	filePointer = fopen("postfix.txt", "r");
	
	if (!filePointer) {
		printf("Can't read file!\n");
		return FILE_ERROR;
	}

	fgets(buffer, MAX_LENGHT, filePointer);
	fclose(filePointer);

	return EXIT_SUCCESS;
}

Position initialization(Position head) {
	Position newElement = NULL;
	newElement = (Position)malloc(sizeof(stack));
	if (!newElement) {
		printf("Can't allocate memory!\n");
		return ALLOC_ERROR;
	}
	newElement->Element = 0;
	newElement->Next = NULL;

	return newElement;
}