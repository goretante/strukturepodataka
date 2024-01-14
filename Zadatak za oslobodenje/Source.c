#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct Book;
struct User;

typedef struct Book {
	char title[50];
	char author[50];
	int year;
	int availableCopies;
	struct User* borrowedBy;
	struct Book* next;
} Book;

typedef struct User {
	char name[50];
	int borrowedBooks;
	struct Book borrowedList[5];
	struct User* next;
} User;

int main() {

	return EXIT_SUCCESS;
}