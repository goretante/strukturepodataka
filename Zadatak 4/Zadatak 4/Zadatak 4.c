#define _CRT_SECURE_NO_WARNINGS

// includes
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// structure
typedef struct Node {
	int coef;
	int exp;
	struct Node* next;
} Node;

// prototype functions
int insertSorted(Node** head, int coef, int exp);
int printPolynomial(Node* poly);
int readPolynomialFromFile(const char* fileName, Node** poly1, Node** poly2);
Node* addPolynomials(Node* poly1, Node* poly2);
Node* multiplyPolynomials(Node* poly1, Node* poly2);
int freeList(Node* head);

// main
int main() {
	Node* poly1 = NULL;
	Node* poly2 = NULL;

	// Èitanje polinoma iz datoteke
	readPolynomialFromFile("polynomes.txt", &poly1, &poly2);

	// Ispisivanje sortiranih polinoma
	printf("Polynomial 1: ");
	printPolynomial(poly1);
	printf("Polynomial 2: ");
	printPolynomial(poly2);

	// Zbrajanje polinoma
	Node* sumResult = addPolynomials(poly1, poly2);
	printf("Sum: ");
	printPolynomial(sumResult);
	freeList(sumResult);

	// Množenje polinoma
	Node* multiplyResult = multiplyPolynomials(poly1, poly2);
	printf("Product: ");
	printPolynomial(multiplyResult);
	freeList(multiplyResult);

	// Oslobaðanje memorije
	freeList(poly1);
	freeList(poly2);

	return EXIT_SUCCESS;
}

int insertSorted(Node** head, int coef, int exp) {
    Node* newNode = (Node*)malloc(sizeof(Node));
    if (!newNode) {
        printf("Memory allocation failed.\n");
        return EXIT_FAILURE;
    }
    newNode->coef = coef;
    newNode->exp = exp;
    newNode->next = NULL;

    Node* current = *head;
    Node* prev = NULL;

    while (current != NULL && current->exp > exp) {
        prev = current;
        current = current->next;
    }

    if (current != NULL && current->exp == exp) {
        current->coef += coef;
        free(newNode);
    }
    else {
        if (prev == NULL) {
            newNode->next = *head;
            *head = newNode;
        }
        else {
            newNode->next = current;
            prev->next = newNode;
        }
    }

    return EXIT_SUCCESS;
}

int printPolynomial(Node* poly) {
    while (poly != NULL) {
        printf("%dx^%d", poly->coef, poly->exp);
        poly = poly->next;
        if (poly != NULL)
            printf(" + ");
    }
    printf("\n");

    return EXIT_SUCCESS;
}

int readPolynomialFromFile(const char* fileName, Node** poly1, Node** poly2) {
    FILE* filePointer = NULL;
    char line[100];

    filePointer = fopen(fileName, "r");
    if (filePointer == NULL) {
        printf("Can't open file!\n");
        return EXIT_FAILURE;
    }

    int isReadingPoly1 = 1;

    while (fgets(line, sizeof(line), filePointer) != NULL) {
        char* token = strtok(line, " ");
        while (token != NULL) {
            int coef, exp;
            if (sscanf(token, "%dx^%d", &coef, &exp) == 2) {
                if (isReadingPoly1) {
                    if (insertSorted(poly1, coef, exp) != EXIT_SUCCESS) {
                        printf("Insert element into the list error.\n");
                        fclose(filePointer);
                        return EXIT_FAILURE;
                    }
                }
                else {
                    if (insertSorted(poly2, coef, exp) != EXIT_SUCCESS) {
                        printf("Insert element into the list error.\n");
                        fclose(filePointer);
                        return EXIT_FAILURE;
                    }
                }
            }
            else {
                printf("Invalid term format: %s\n", token);
                fclose(filePointer);
                return EXIT_FAILURE;
            }

            token = strtok(NULL, " ");
        }

        // Promjena polinoma nakon svake linije
        isReadingPoly1 = !isReadingPoly1;
    }

    fclose(filePointer);
    return EXIT_SUCCESS;
}

Node* addPolynomials(Node* poly1, Node* poly2) {
    Node* result = NULL;
    int coef1, exp1, coef2, exp2, sumCoef, sumExp;

    while (poly1 != NULL || poly2 != NULL) {
        coef1 = (poly1 != NULL) ? poly1->coef : 0;
        exp1 = (poly1 != NULL) ? poly1->exp : 0;
        coef2 = (poly2 != NULL) ? poly2->coef : 0;
        exp2 = (poly2 != NULL) ? poly2->exp : 0;

        sumCoef = coef1 + coef2;

        if (exp1 > exp2) sumExp = exp1;
        else sumExp = exp2;

        insertSorted(&result, sumCoef, sumExp);

        if (poly1 != NULL) poly1 = poly1->next;
        if (poly2 != NULL) poly2 = poly2->next;
    }

    return result;
}

Node* multiplyPolynomials(Node* poly1, Node* poly2) {
	Node* result = NULL;

	while (poly1 != NULL) {
		Node* temp = poly2;
		while (temp != NULL) {
			int coef = poly1->coef * temp->coef;
			int exp = poly1->exp + temp->exp;

			insertSorted(&result, coef, exp);

			temp = temp->next;
		}

		poly1 = poly1->next;
	}

	return result;
}

int freeList(Node* head) {
	Node* current = head;
	Node* next;

	while (current != NULL) {
		next = current->next;
		free(current);
		current = next;
	}

	return EXIT_SUCCESS;
}
