/*
"BookHaven" je knjižnica koja želi unaprijediti svoj sustav praæenja knjiga, autora i korisnika.
Svaka knjiga ima informacije o naslovu, autoru, godini izdanja i dostupnim primjercima.
Korisnici mogu posuðivati knjige i vratiti ih nakon nekog vremena.
Program mora korisniku omoguæiti:
	a) ispis svih knjiga abecedno (tako da se mogu vidjeti svi podatci o knjizi i korisnici koji trenutno posuðuju knjigu) - done
	b) ispis svih korisnika abecedno (tako da se mogu vidjeti sve knjige koje je neki korisnik posudio) - done
	c) pretraživanje knjiga po godini izdanja (tako da se mogu vidjeti sve knjige iz te godine i njihova kolièina) - done
	d) pretraživanje knjiga po nazivu autora (tako da se mogu vidjeti sve knjige tog autora i njihova kolièina) - done
	e) unos novog korisnika - done
	f) posudba knjige korisniku - napokon gotovo
		- može više primjeraka iste knjige posuditi od puta
		- korisnik u sebe nikad ne smije imati više od 5 knjiga sve skupa
		- ako pokušava napravit posudbu koja æe ga dovesti preko tog broja program treba upozoriti korisnika na to i ispisati mu broj koliko još knjiga posudit
		- ako korisnik pita za više primjeraka knjige, a knjižnica na stanju nema nema dovoljno treba korisniku omoguæiti izbor hoæe li posudit to što ima ili ne.
	g) povratak knjiga na stanje
	h) spremi (u datoteku/datoteke spremiti trenutno stanje u knjižnici, tako da kad se iduæi put pokrene program moguæe nastaviti od spremljenog stanja)
*/

#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct Book;
struct User;

typedef struct BorrowedBook {
	struct Book* book;
	struct User* borrower;
	struct BorrowedBook* next;
} BorrowedBook;

typedef struct Borrower {
	struct User* user;
	struct Borrower* next;
} Borrower;

typedef struct Book {
	char title[50];
	char author[50];
	int year;
	int copies;
	Borrower* borrowers;
	struct Book* next;
} Book;

typedef struct User {
	char name[50];
	int borrowedBooks;
	BorrowedBook* borrowedBooksList;
	struct User* next;
} User;

Book* addBookSorted(Book* head, const char* title, const char* author, int year, int copies);
Book* addNewBookSorted(Book* head);
User* addUserSorted(User* head, const char* name);
User* addNewUserSorted(User* head);
int printBooks(Book* head);
int printUsers(User* head);
int searchBookByYear(Book* head);
int searchBookByAuthor(Book* head);
Book* findBook(Book* bookHead, const char* bookname);
User* findUser(User* userHead, const char* username);
int borrowBook(Book* book, User* user, const int numBooks);
int borrowBookInput(User* userHead, Book* bookHead);
int returnBook(User* user, Book* book, int numBooks);
int returnBooksInput(User* userHead, Book* bookHead);
int saveToFile(User* userHead, Book* bookHead);
void freeMemory(Book* bookHead, User* userHead);


int main() {
	Book* bookHead = NULL;
	User* userHead = NULL;


	int choice;
	do {
		printf("\nOptions:\n");
		printf("1. Add new book\n");
		printf("2. Add new user\n");
		printf("3. Print books\n");
		printf("4. Print users\n");
		printf("5. Search book by year\n");
		printf("6. Search book by author\n");
		printf("7. Borrow books\n");
		printf("8. Return books\n");
		printf("9. Save data\n");
		printf("0. Exit\n");


		printf("Enter your choice: ");
		scanf("%d", &choice);

		switch (choice) {
		case 1:
			bookHead = addNewBookSorted(bookHead);
			break;
		case 2:
			userHead = addNewUserSorted(userHead);
			break;
		case 3:
			printBooks(bookHead);
			break;
		case 4:
			printUsers(userHead);
			break;
		case 5:
			searchBookByYear(bookHead);
			break;
		case 6:
			searchBookByAuthor(bookHead);
			break;
		case 7:
			borrowBookInput(userHead, bookHead);
			break;
		case 8:
			returnBooksInput(userHead, bookHead);
			break;
		case 9:
			saveToFile(userHead, bookHead);
			break;
		case 0:
			freeMemory(bookHead, userHead);
			break;
		default:
			printf("Wrong option! Try again.\n");
		}
	} while (choice != 0);


	return 0;
}

Book* addBookSorted(Book* head, const char* title, const char* author, int year, int copies) {
	Book* newBook = (Book*)malloc(sizeof(Book));
	if (!newBook) {
		perror("Memory allocation error!\n");
		return NULL;
	}

	strcpy(newBook->title, title);
	strcpy(newBook->author, author);
	newBook->year = year;
	newBook->copies = copies;
	newBook->borrowers = NULL;

	if (head == NULL || strcmp(title, head->title) < 0) {
		newBook->next = head;
		return newBook;
	}

	Book* current = head;
	while (current->next != NULL && strcmp(title, current->next->title) > 0) {
		current = current->next;
	}

	newBook->next = current->next;
	current->next = newBook;

	return head;
}

Book* addNewBookSorted(Book* head) {
	char title[50];
	char author[50];
	int year;
	int copies;

	printf("---------------\n");

	printf("Enter the title of the book: ");
	scanf(" %[^\n]", title);

	printf("Enter the author of the book: ");
	scanf(" %[^\n]", author);

	printf("Enter the year of publication: ");
	scanf("%d", &year);

	printf("Enter the number of available copies: ");
	scanf("%d", &copies);

	head = addBookSorted(head, title, author, year, copies);

	printf("Book added successfully.\n");

	return head;
}

User* addUserSorted(User* head, const char* name) {
	User* newUser = (User*)malloc(sizeof(User));
	if (!newUser) {
		printf("Error allocating memory!\n");
		return NULL;
	}

	strcpy(newUser->name, name);
	newUser->borrowedBooks = 0;
	newUser->borrowedBooksList = NULL;

	if (head == NULL || strcmp(name, head->name) < 0) {
		newUser->next = head;
		return newUser;
	}

	User* current = head;
	while (current->next != NULL && strcmp(name, current->next->name) > 0) {
		current = current->next;
	}

	newUser->next = current->next;
	current->next = newUser;

	return head;
}

User* addNewUserSorted(User* head) {
	char name[50];

	printf("---------------\n");

	printf("Enter the name of the user: ");
	scanf(" %[^\n]", name);

	head = addUserSorted(head, name);

	printf("User added successfully!\n");

	return head;
}

int printBooks(Book* head) {
	Book* current = head;

	printf("---------------\n");

	while (current != NULL) {
		printf("Title: %s\n", current->title);
		printf("Author: %s\n", current->author);
		printf("Year: %d\n", current->year);
		printf("Available copies: %d\n", current->copies);

		if (current->borrowers != NULL) {
			printf("Borrowed by:\n");
			Borrower* borrower = current->borrowers;
			while (borrower != NULL) {
				printf(" - %s\n", borrower->user->name);
				borrower = borrower->next;
			}
		}
		printf("\n");

		current = current->next;
	}

	return EXIT_SUCCESS;
}

int printUsers(User* head) {
	User* current = head;

	printf("---------------\n");

	while (current != NULL) {
		printf("Name: %s\n", current->name);
		printf("Number of borrowed books: %d\n", current->borrowedBooks);

		if (current->borrowedBooks > 0) {
			printf("Borrowed books:\n");
			BorrowedBook* borrowedBook = current->borrowedBooksList;
			for (int i = 0; i < current->borrowedBooks; i++) {
				printf("  - %s\n", borrowedBook->book->title);
				borrowedBook = borrowedBook->next;
			}
		}

		printf("\n");

		current = current->next;
	}

	return EXIT_SUCCESS;
}

int searchBookByYear(Book* head) {
	Book* current = head;
	int year;
	int found = 0;

	printf("---------------\n");

	printf("Enter the year: ");
	scanf("%d", &year);

	printf("---------------\n");

	printf("Books published in %d:\n", year);

	while (current != NULL) {
		if (current->year == year) {
			printf("Title: %s\n", current->title);
			printf("Author: %s\n", current->author);
			printf("Year: %d\n", current->year);
			printf("Available copies: %d\n", current->copies);

			Borrower* borrower = current->borrowers;
			if (borrower != NULL) {
				printf("Borrowed by:\n");
				while (borrower != NULL) {
					printf(" - %s\n", borrower->user->name);
					borrower = borrower->next;
				}
			}
			printf("\n");
			found = 1;
		}
		current = current->next;
	}

	if (!found)
		printf("No books found for the given year.\n");

	return EXIT_SUCCESS;
}

int searchBookByAuthor(Book* head) {
	Book* current = head;
	char author[50];
	int found = 0;

	printf("---------------\n");

	printf("Enter the author's name: ");
	scanf(" %[^\n]", author);

	printf("---------------\n");

	printf("Books written by %s:\n", author);

	while (current != NULL) {
		if (strcmp(current->author, author) == 0) {
			printf("Title: %s\n", current->title);
			printf("Author: %s\n", current->author);
			printf("Year: %d\n", current->year);
			printf("Available copies: %d\n", current->copies);

			Borrower* borrower = current->borrowers;
			if (borrower != NULL) {
				printf("Borrowed by:\n");
				while (borrower != NULL) {
					printf(" - %s\n", borrower->user->name);
					borrower = borrower->next;
				}
			}
			printf("\n");
			found = 1;
		}
		current = current->next;
	}

	if (!found) {
		printf("No books found for the given author.\n");
	}

	return EXIT_SUCCESS;
}

Book* findBook(Book* bookHead, const char* bookname) {
	Book* current = bookHead;

	while (current != NULL) {
		if (strcmp(current->title, bookname) == 0) {
			return current;
		}
		current = current->next;
	}

	return NULL;
}

User* findUser(User* userHead, const char* username) {
	User* current = userHead;

	while (current != NULL) {
		if (strcmp(current->name, username) == 0) {
			return current;
		}
		current = current->next;
	}

	return NULL;
}

int borrowBook(Book* book, User* user, int numBooks) {
	int i, checkamount = 0;
	int noBooksOnStart = book->copies;

	if (user->borrowedBooks >= 5) {
		printf("You've already borrowed the maximum number of books!\n");
		return EXIT_FAILURE;
	}

	if ((user->borrowedBooks + numBooks) > 5) {
		printf("You will borrow more books than you can have!\n");
		printf("You can borrow %d more books.\n", 5 - user->borrowedBooks);
		return EXIT_FAILURE;
	}

	if (book->copies <= 0) {
		printf("Currently we are out of these books!\n");
		return EXIT_FAILURE;
	}

	checkamount = book->copies - numBooks;
	if (checkamount < 0) {
		char choice;
		printf("We have fewer books than you want to borrow (%d).\n", book->copies);
		printf("Do you want them all? (Y/N): ");
		scanf(" %c", &choice);
		switch (choice) {
		case 'Y':
		case 'y':
			for (i = 0; i < noBooksOnStart; i++) {
				BorrowedBook* borrowedBook = (BorrowedBook*)malloc(sizeof(BorrowedBook));
				borrowedBook->book = book;
				borrowedBook->borrower = user;
				borrowedBook->next = user->borrowedBooksList;
				user->borrowedBooksList = borrowedBook;

				user->borrowedBooks++;
				book->copies--;

				Borrower* borrower = (Borrower*)malloc(sizeof(Borrower));
				borrower->user = user;
				borrower->next = book->borrowers;
				book->borrowers = borrower;
			}
			printf("%d book(s) by %s borrowed successfully by %s.\n", noBooksOnStart, book->title, user->name);
			return EXIT_SUCCESS;
			break;
		case 'N':
		case 'n':
			printf("You've given up on borrowing the book.\n");
			return EXIT_SUCCESS;
		}
	}

	for (i = 0; i < numBooks; i++) {
		BorrowedBook* borrowedBook = (BorrowedBook*)malloc(sizeof(BorrowedBook));
		borrowedBook->book = book;
		borrowedBook->borrower = user;
		borrowedBook->next = user->borrowedBooksList;
		user->borrowedBooksList = borrowedBook;

		user->borrowedBooks++;
		book->copies--;

		Borrower* borrower = (Borrower*)malloc(sizeof(Borrower));
		borrower->user = user;
		borrower->next = book->borrowers;
		book->borrowers = borrower;
	}

	printf("%d book(s) by %s borrowed successfully by %s.\n", numBooks, book->title, user->name);
	return EXIT_SUCCESS;
}

int borrowBookInput(User* userHead, Book* bookHead) {
	char username[50] = { 0 };
	char bookname[50] = { 0 };
	int nobooks = 0;
	User* currentUser = NULL;
	Book* currentBook = NULL;

	printf("---------------\n");

	printf("Enter your name: ");
	scanf(" %[^\n]", username);

	currentUser = findUser(userHead, username);
	if (!currentUser) {
		printf("User not found!\n");
		return EXIT_FAILURE;
	}

	printf("Enter the title of the book you want to borrow: ");
	scanf(" %[^\n]", bookname);

	currentBook = findBook(bookHead, bookname);
	if (!currentBook) {
		printf("Book not found!\n");
		return EXIT_FAILURE;
	}

	printf("Amount of books you want to borrow (1 - 5): ");
	scanf("%d", &nobooks);

	borrowBook(currentBook, currentUser, nobooks);

	return EXIT_SUCCESS;
}

int returnBook(User* user, Book* book, int numBooks) {
	BorrowedBook* borrowedBook = user->borrowedBooksList;
	BorrowedBook* prevBorrowedBook = NULL;
	int booksReturned = 0;

	while (borrowedBook != NULL && booksReturned < numBooks) {
		if (borrowedBook->book == book) {
			book->copies++;

			if (prevBorrowedBook != NULL) {
				prevBorrowedBook->next = borrowedBook->next;
			}
			else {
				user->borrowedBooksList = borrowedBook->next;
			}

			BorrowedBook* temp = borrowedBook;
			borrowedBook = borrowedBook->next;
			free(temp);

			Borrower* currentBorrower = book->borrowers;
			Borrower* prevBorrower = NULL;
			while (currentBorrower != NULL) {
				if (currentBorrower->user == user) {
					if (prevBorrower != NULL) {
						prevBorrower->next = currentBorrower->next;
					}
					else {
						book->borrowers = currentBorrower->next;
					}
					free(currentBorrower);
					break;
				}
				prevBorrower = currentBorrower;
				currentBorrower = currentBorrower->next;
			}

			booksReturned++;
		}
		else {
			prevBorrowedBook = borrowedBook;
			borrowedBook = borrowedBook->next;
		}
	}

	user->borrowedBooks -= booksReturned;

	return booksReturned;
}

int returnBooksInput(User* userHead, Book* bookHead) {
	char username[50] = { 0 };
	char bookname[50] = { 0 };
	int numBooks = 0;
	User* currentUser = NULL;
	Book* currentBook = NULL;

	printf("---------------\n");

	printf("Enter your name: ");
	scanf(" %[^\n]", username);

	currentUser = findUser(userHead, username);
	if (!currentUser) {
		printf("User not found!\n");
		return EXIT_FAILURE;
	}

	printf("Enter the title of the book you want to return: ");
	scanf(" %[^\n]", bookname);

	currentBook = findBook(bookHead, bookname);
	if (!currentBook) {
		printf("Book not found!\n");
		return EXIT_FAILURE;
	}

	printf("Amount of books you want to return (1 - 5): ");
	scanf("%d", &numBooks);

	int booksReturned = returnBook(currentUser, currentBook, numBooks);

	printf("%d book(s) returned successfully by %s.\n", booksReturned, currentUser->name);

	return EXIT_SUCCESS;
}

int saveToFile(User* userHead, Book* bookHead) {
	User* currentUser = userHead;
	Book* currentBook = bookHead;
	FILE* filePointer = NULL;

	filePointer = fopen("books.txt", "w");
	if (!filePointer) {
		printf("Could not open the file!\n");
		return EXIT_FAILURE;
	}

	while (currentBook != NULL) {
		fprintf(filePointer, "%s;%s;%d;%d;", currentBook->title, currentBook->author, currentBook->year, currentBook->copies);
		while (currentBook->borrowers != NULL) {
			fprintf(filePointer, "%s,", currentBook->borrowers->user->name);
			currentBook->borrowers = currentBook->borrowers->next;
		}
		fprintf(filePointer, "\n");
		currentBook = currentBook->next;
	}

	fclose(filePointer);

	filePointer = fopen("users.txt", "w");
	if (!filePointer) {
		printf("Could not open the file!\n");
		return EXIT_FAILURE;
	}

	while (currentUser != NULL) {
		fprintf(filePointer, "%s;%d;", currentUser->name, currentUser->borrowedBooks);
		while (currentUser->borrowedBooksList != NULL) {
			fprintf(filePointer, "%s,", currentUser->borrowedBooksList->book->title);
			currentUser->borrowedBooksList = currentUser->borrowedBooksList->next;
		}
		fprintf(filePointer, "\n");
	}

	fclose(filePointer);

	return EXIT_SUCCESS;
}

void freeMemory(Book* bookHead, User* userHead) {
	while (bookHead != NULL) {
		Book* tempBook = bookHead;
		bookHead = bookHead->next;

		Borrower* borrower = tempBook->borrowers;
		while (borrower != NULL) {
			Borrower* tempBorrower = borrower;
			borrower = borrower->next;
			free(tempBorrower);
		}

		free(tempBook);
	}

	while (userHead != NULL) {
		User* tempUser = userHead;
		userHead = userHead->next;

		BorrowedBook* borrowedBook = tempUser->borrowedBooksList;
		while (borrowedBook != NULL) {
			BorrowedBook* tempBorrowedBook = borrowedBook;
			borrowedBook = borrowedBook->next;
			free(tempBorrowedBook);
		}

		free(tempUser);
	}
}
