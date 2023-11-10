#define _CRT_SECURE_NO_WARNINGS

// includes
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// defines
#define MAX_LENGTH 50

// structure
struct _stackElement;
typedef struct _stackElement* Position;
typedef struct _stackElement {
	double number;
	Position next;
} StackElement;

// function prototypes
int calculatePostfixFromFile(Position head, char* fileName, double* result);
int readFile(char* fileName, char* buffer);
int parseStringIntoPostfix(Position head, char* buffer, double* result);
int checkStackAndExtractResult(Position head, double* result);
Position createStackElement(double number);
int popAndPerformOperation(Position head, char* operation, double* result);

// main
int main() {

	StackElement head = { .number = 0, .next = NULL };
	double result = 0;

	if (calculatePostfixFromFile(&head, "postfix.txt", &result) == EXIT_SUCCESS) {
		printf("Result: %.1lf\n", result);
	}


	return EXIT_SUCCESS;
}

int calculatePostfixFromFile(Position head, char* fileName, double* result) {

	char buffer[MAX_LENGTH] = { 0 };
	int status = 0;

	if (readFile(fileName, buffer) || !buffer) {
		return EXIT_FAILURE;
	}

	status = parseStringIntoPostfix(head, buffer, result);
	if (status != EXIT_SUCCESS) {
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}

int readFile(char* fileName, char* buffer) {

	FILE* filePointer = NULL;

	filePointer = fopen(fileName, "r");
	if (!filePointer) {
		return EXIT_FAILURE;
	}

	fgets(buffer, MAX_LENGTH, filePointer);
	printf("|%s|\n", buffer);

	fclose(filePointer);

	return EXIT_SUCCESS;
}

int parseStringIntoPostfix(Position head, char* buffer, double* result) {

	char* currentBuffer = buffer;
	int status = 0;
	int numBytes = 0;
	char operation = '\0';
	double number = 0.0;
	Position newStackElement = NULL;

	while (strlen(currentBuffer) > 0) {
		status = sscanf(currentBuffer, " %lf %n", &number, &numBytes);
		if (status != 1) {
			sscanf(currentBuffer, " %c %n", &operation, &numBytes);
			status = popAndPerformOperation(head, operation, result);

			if (status != EXIT_SUCCESS) {
				return EXIT_FAILURE;
			}
			
			number = *result;
		}

		newStackElement = createStackElement(number);
		if (!newStackElement) {
			return EXIT_FAILURE;
		}

		currentBuffer += numBytes;
		printf("|%s| <-->", currentBuffer);
		push(head, newStackElement);
	}
	
	return checkStackAndExtractResult(head, result);
}

int checkStackAndExtractResult(Position head, double* result) {


	int status = EXIT_SUCCESS;

	status = pop(head, result);

	if (status != EXIT_SUCCESS) {
		return status;
	}

	if (head->next) {
		printf("Invalid postfix, check file.\n");
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}

Position createStackElement(double number) {

	Position newElement = NULL;

	newElement = (Position)malloc(sizeof(StackElement));
	if (!newElement) {
		printf("Can't allocate memory!\n");
		return NULL;
	}

	newElement->number = number;
	newElement->next = NULL;

	return newElement;
}

int push(Position head, Position newStackElement) {
	
	newStackElement->next = head->next;
	head->next = newStackElement;

	printStack(head->next);

	return EXIT_SUCCESS;
}

int printStack(Position first) {
	
	Position current = first;

	while (current) {
		printf(" %.1lf", current->number);
		current = current->next;
	}
	
	printf("\n");
	return EXIT_SUCCESS;
}

int pop(Position head, double* result) {
	
	Position toDelete = NULL;

	toDelete = head->next;
	if (!toDelete) {
		printf("Stack is empty.\n");
		return -1;
	}

	head->next = toDelete->next;
	*result = toDelete->number;
	free(toDelete);

	return EXIT_SUCCESS;
}

int popAndPerformOperation(Position head, char operation, double* result) {

	double operand1 = 0;
	double operand2 = 0;
	int status1 = 0;
	int status2 = 0;

	status1 = pop(head, &operand1);
	if (status1 != EXIT_SUCCESS) {
		return EXIT_FAILURE;
	}

	status2 = pop(head, &operand2);
	if (status2 != EXIT_SUCCESS) {
		return EXIT_FAILURE;
	}

	switch (operation) {
	case '+':
		*result = operand2 + operand1;
		break;
	case '-':
		*result = operand2 - operand1;
		break;
	case '*':
		*result = operand2 * operand1;
		break;
	case '/':
		*result = operand2 / operand1;
		break;
	default:
		printf("Can't do operation %c.\r\n", operation);
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}