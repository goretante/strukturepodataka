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
    int copies;
    struct User* borrower;
    struct Book* next;
} Book;

typedef struct User {
    char name[50];
    int borrowedBooks;
    struct Book* borrowedBooksList[5];
    struct User* next;
} User;

// function prototypes
void printBooks(Book* head);
void printUsers(User* head);
Book* addBookSorted(Book* head, const char* title, const char* author, int year, int copies);
Book* addNewBookSorted(Book* head);
Book* findBook(Book* head, const char* title);
User* addUserSorted(User* head, const char* name);
User* addNewUserSorted(User* head);
User* findUser(User* head, const char* name);
void searchBooksByYear(Book* head, int year);
void searchBooksByAuthor(Book* head, const char* author);
void borrowBook(User* user, Book* book);
void borrowBookFromUserInput(User* userHead, Book* bookHead);
void returnBook(User* user, Book* book);
void returnBookFromUserInput(User* userHead, Book* bookHead);
void saveStateToFile(Book* bookHead, User* userHead);
void loadStateFromFile(Book** bookHead, User** userHead);

// main
int main() {
    Book* bookHead = NULL;
    User* userHead = NULL;

    loadStateFromFile(&bookHead, &userHead);

    int choice;
    do {
        printf("\nOptions:\n");
        printf("1. Print all books alphabetically\n");
        printf("2. Print all users alphabetically\n");
        printf("3. Search books by year\n");
        printf("4. Search books by author\n");
        printf("5. Add a new user\n");
        printf("6. Borrow a book\n");
        printf("7. Return a book\n");
        printf("8. Save state\n");
        printf("9. Add new book\n");
        printf("0. Exit\n");

        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
        case 1:
            printBooks(bookHead);
            break;
        case 2:
            printUsers(userHead);
            break;
        case 3:
            printf("Enter the year: ");
            int searchYear;
            scanf("%d", &searchYear);
            searchBooksByYear(bookHead, searchYear);
            break;
        case 4:
            printf("Enter the author's name: ");
            char searchAuthor[50];
            scanf(" %[^\n]", searchAuthor);
            searchBooksByAuthor(bookHead, searchAuthor);
            break;
        case 5:
            userHead = addNewUserSorted(userHead);
            break;
        case 6:
            borrowBookFromUserInput(userHead, bookHead);
            break;
        case 7:
            returnBookFromUserInput(userHead, bookHead);
            break;
        case 8:
            saveStateToFile(bookHead, userHead);
            break;
        case 9:
            bookHead = addNewBookSorted(bookHead);
            break;
        case 0:
            // Izlaz iz programa
            break;
        default:
            printf("Invalid choice. Please enter a valid option.\n");
        }

    } while (choice != 0);

    // Oslobađanje memorije prije izlaska iz programa
    // freeMemory(bookHead, userHead);

    return EXIT_SUCCESS;
}


// functions
void printBooks(Book * head) {
    Book* current = head;

    while (current != NULL) {
        printf("Title: %s\n", current->title);
        printf("Author: %s\n", current->author);
        printf("Year: %d\n", current->year);
        printf("Available Copies: %d\n", current->copies);

        if (current->borrower != NULL) {
            printf("Borrowed by: %s\n", current->borrower->name);
        }

        printf("\n");

        current = current->next;
    }
}

void printUsers(User* head) {
    if (head == NULL) {
        printf("No users found.\n");
        return;
    }

    printf("Users:\n");
    printf("------\n");

    User* current = head;

    while (current != NULL) {
        printf("Name: %s\n", current->name);
        printf("Borrowed books: %d\n", current->borrowedBooks);

        if (current->borrowedBooks > 0) {
            printf("Borrowed Books:\n");
            for (int i = 0; i < current->borrowedBooks; i++) {
                printf("  - %s\n", current->borrowedBooksList[i]->title);
            }
        }

        printf("\n");

        current = current->next;
    }
}

Book* addBookSorted(Book* head, const char* title, const char* author, int year, int copies) {
    Book* newBook = (Book*)malloc(sizeof(Book));
    if (!newBook) {
        perror("Memory allocation error");
        exit(EXIT_FAILURE);
    }

    strcpy(newBook->title, title);
    strcpy(newBook->author, author);
    newBook->year = year;
    newBook->copies = copies;
    newBook->borrower = NULL;

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
    int year, copies;

    printf("Enter the title of the book: ");
    scanf(" %[^\n]", title);

    printf("Enter the author of the book: ");
    scanf(" %[^\n]", author);

    printf("Enter the year of publication: ");
    scanf("%d", &year);

    printf("Enter the number of copies available: ");
    scanf("%d", &copies);

    head = addBookSorted(head, title, author, year, copies);

    printf("Book added successfully!\n");

    return head;
}

Book* findBook(Book* head, const char* title) {
    Book* current = head;
    while (current != NULL) {
        if (strcmp(current->title, title) == 0) {
            return current;
        }
        current = current->next;
    }
    return NULL;
}

User* addUserSorted(User* head, const char* name) {
    User* newUser = (User*)malloc(sizeof(User));
    if (!newUser) {
        perror("Memory allocation error");
        exit(EXIT_FAILURE);
    }

    strcpy(newUser->name, name);
    newUser->borrowedBooks = 0;

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

    printf("Enter the name of the user: ");
    scanf(" %[^\n]", name);

    head = addUserSorted(head, name);

    printf("User added successfully!\n");

    return head;
}

User* findUser(User* head, const char* name) {
    User* current = head;
    while (current != NULL) {
        if (strcmp(current->name, name) == 0) {
            return current;
        }
        current = current->next;
    }
    return NULL;
}

void searchBooksByYear(Book* head, int year) {
    Book* current = head;
    int found = 0;

    printf("Books published in %d:\n", year);
    printf("----------------------\n");

    while (current != NULL) {
        if (current->year == year) {
            printf("Title: %s\n", current->title);
            printf("Author: %s\n", current->author);
            printf("Available Copies: %d\n", current->copies);

            if (current->borrower != NULL) {
                printf("Borrowed by: %s\n", current->borrower->name);
            }

            printf("\n");

            found = 1;
        }

        current = current->next;
    }

    if (!found) {
        printf("No books found for the given year.\n");
    }
}

void searchBooksByAuthor(Book* head, const char* author) {
    Book* current = head;
    int found = 0;

    printf("Books by author %s:\n", author);
    printf("---------------------\n");

    while (current != NULL) {
        if (strcmp(current->author, author) == 0) {
            printf("Title: %s\n", current->title);
            printf("Year: %d\n", current->year);
            printf("Available Copies: %d\n", current->copies);

            if (current->borrower != NULL) {
                printf("Borrowed by: %s\n", current->borrower->name);
            }

            printf("\n");

            found = 1;
        }

        current = current->next;
    }

    if (!found) {
        printf("No books found for the given author.\n");
    }
}

void borrowBook(User* user, Book* book) {
    if (user->borrowedBooks >= 5) {
        printf("You have already borrowed the maximum number of books.\n");
        return;
    }

    if (book->copies <= 0) {
        printf("Sorry, the book '%s' by %s is currently not available.\n", book->title, book->author);
        return;
    }

    user->borrowedBooksList[user->borrowedBooks] = book;
    user->borrowedBooks++;

    book->copies--;

    book->borrower = user;

    printf("Book '%s' by %s borrowed successfully by %s.\n", book->title, book->author, user->name);
}

void borrowBookFromUserInput(User* userHead, Book* bookHead) {
    printf("Enter your name: ");
    char userName[50];
    scanf(" %[^\n]", userName);

    User* currentUser = findUser(userHead, userName);

    if (currentUser == NULL) {
        printf("User not found.\n");
        return;
    }

    printf("Enter the title of the book you want to borrow: ");
    char bookTitle[50];
    scanf(" %[^\n]", bookTitle);

    Book* currentBook = bookHead;

    while (currentBook != NULL) {
        if (strcmp(currentBook->title, bookTitle) == 0) {
            borrowBook(currentUser, currentBook);
            return;
        }

        currentBook = currentBook->next;
    }

    printf("Book not found.\n");
}

void returnBook(User* user, Book* book) {
    int found = 0;
    for (int i = 0; i < user->borrowedBooks; i++) {
        if (user->borrowedBooksList[i] == book) {
            found = 1;

            for (int j = i; j < user->borrowedBooks - 1; j++) {
                user->borrowedBooksList[j] = user->borrowedBooksList[j + 1];
            }

            user->borrowedBooks--;

            book->copies++;

            book->borrower = NULL;

            printf("Book '%s' returned successfully by %s.\n", book->title, user->name);
            break;
        }
    }

    if (!found) {
        printf("You didn't borrow the book '%s' by %s.\n", book->title, book->author);
    }
}

// Funkcija za vraćanje knjige, koja uzima unos korisnika i knjige od korisnika
void returnBookFromUserInput(User* userHead, Book* bookHead) {
    printf("Enter your name: ");
    char userName[50];
    scanf(" %[^\n]", userName);

    // Pronađi korisnika
    User* currentUser = findUser(userHead, userName);

    if (currentUser == NULL) {
        printf("User not found.\n");
        return;
    }

    printf("Enter the title of the book you want to return: ");
    char bookTitle[50];
    scanf(" %[^\n]", bookTitle);

    // Pronađi knjigu
    Book* currentBook = bookHead;

    while (currentBook != NULL) {
        if (strcmp(currentBook->title, bookTitle) == 0) {
            returnBook(currentUser, currentBook);
            return;
        }

        currentBook = currentBook->next;
    }

    printf("Book not found.\n");
}

void saveStateToFile(Book* bookHead, User* userHead) {
    // Spremi knjige
    FILE* booksFile = fopen("books.txt", "w");
    if (booksFile == NULL) {
        perror("Error opening books file");
        exit(EXIT_FAILURE);
    }

    while (bookHead != NULL) {
        fprintf(booksFile, "%s;%s;%d;%d\n", bookHead->title, bookHead->author, bookHead->year, bookHead->copies);
        bookHead = bookHead->next;
    }

    fclose(booksFile);

    // Spremi korisnike
    FILE* usersFile = fopen("users.txt", "w");
    if (usersFile == NULL) {
        perror("Error opening users file");
        exit(EXIT_FAILURE);
    }

    while (userHead != NULL) {
        fprintf(usersFile, "%s;%d;", userHead->name, userHead->borrowedBooks);

        // Spremi posuđene knjige korisnika odvojene zarezima
        for (int i = 0; i < userHead->borrowedBooks; i++) {
            fprintf(usersFile, "%s", userHead->borrowedBooksList[i]->title);

            if (i < userHead->borrowedBooks - 1) {
                fprintf(usersFile, ",");
            }
        }

        fprintf(usersFile, "\n");

        userHead = userHead->next;
    }

    fclose(usersFile);

    printf("Data saved successfully.\n");
}

void loadStateFromFile(Book** bookHead, User** userHead) {
    // Učitavanje knjiga
    FILE* booksFile = fopen("books.txt", "r");
    if (booksFile == NULL) {
        perror("Error opening books file");
        exit(EXIT_FAILURE);
    }

    char title[50];
    char author[50];
    int year, copies;

    while (fscanf(booksFile, "%49[^;];%49[^;];%d;%d\n", title, author, &year, &copies) == 4) {
        *bookHead = addBookSorted(*bookHead, title, author, year, copies);
    }

    fclose(booksFile);

    // Učitavanje korisnika
    //FILE* usersFile = fopen("users.txt", "r");
    //if (usersFile == NULL) {
    //    perror("Error opening users file");
    //    exit(EXIT_FAILURE);
    //}

    //char line[256]; // Prilagodite veličinu prema potrebi

    //while (fgets(line, sizeof(line), usersFile) != NULL) {
    //    // Učitavanje korisnika
    //    char userName[50];
    //    int borrowedBooks;

    //    sscanf(line, "%49[^;];%d;", userName, &borrowedBooks);

    //    User* currentUser = addUserSorted(userHead, userName);
    //    currentUser->borrowedBooks = borrowedBooks;

    //    // Učitavanje posuđenih knjiga korisnika
    //    if (borrowedBooks > 0) {
    //        char* token = strtok(line, ";");
    //        for (int i = 0; i < 2; i++) {
    //            token = strtok(NULL, ";");  // Preskoči prva dva polja (ime i broj posuđenih knjiga)
    //        }

    //        for (int i = 0; i < borrowedBooks; i++) {
    //            token = strtok(NULL, ";");
    //            Book* borrowedBook = findBook(*bookHead, token);

    //            if (borrowedBook != NULL) {
    //                borrowBook(currentUser, borrowedBook);
    //            }
    //        }
    //    }
    //}

    //fclose(usersFile);

    printf("Data loaded successfully.\n");
}