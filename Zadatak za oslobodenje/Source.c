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

int printBooks(Book* bookList);

int main() {

	return EXIT_SUCCESS;
}

int printBooks(Book* bookList) {
	Book* currentBook = bookList;

	while (currentBook != NULL) {
		printf("Title: %s\nAuthor: %s\nYear: %d\nAvailable copies: %d\n", currentBook->title, currentBook->author, currentBook->year, currentBook->availableCopies);

		if (currentBook->borrowedBy != NULL) {
			printf("Borrowed by:\n");

			User* currentUser = currentBook->borrowedBy;
			while (currentUser != NULL) {
				printf(" - %s\n", currentUser->name);
			}
		}
		else {
			printf("Not borrowed by any user!\n");
		}

		printf("----------------------------\n");

		currentBook = currentBook->next;
	}
}