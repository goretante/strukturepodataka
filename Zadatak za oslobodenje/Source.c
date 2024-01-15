#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <string.h>


// structures
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

// function prototypes
int printBooks(Book* bookList);
int printUsers(User* userList);

// main
int main() {

	return EXIT_SUCCESS;
}


// functions
int printBooks(Book* bookList) {
	Book* currentBook = bookList;

	while (currentBook != NULL) {
		printf("Title: %s\nAuthor: %s\nYear: %d\nAvailable copies: %d\n",
			currentBook->title, currentBook->author, currentBook->year, currentBook->availableCopies);

		if (currentBook->borrowedBy != NULL) {
			printf("Borrowed by:\n");

			User* currentUser = currentBook->borrowedBy;
			while (currentUser != NULL) {
				printf(" - %s\n", currentUser->name);
				currentUser = currentUser->next;
			}
		}
		else {
			printf("Not borrowed by any user!\n");
		}

		printf("----------------------------\n");

		currentBook = currentBook->next;
	}

	return EXIT_SUCCESS;
}

int printUsers(User* userList) {
	User* currentUser = userList;

	while (currentUser != NULL) {
		printf("User: %s\nBorrowed Books: %d\n",
			currentUser->name, currentUser->borrowedBooks);

		if (currentUser->borrowedBooks > 0) {
			printf("Borrowed Books:\n");

			for (int i = 0; i < currentUser->borrowedBooks; i++) {
				printf("- %s\n", currentUser->borrowedList[i].title);
			}
		}
		else {
			printf("No books borrowed.\n");
		}

		printf("----------------------------\n");

		currentUser = currentUser->next;
	}

	return EXIT_SUCCESS;
}