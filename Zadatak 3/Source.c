#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAXSIZE 50
#define SUCCESS 0
#define NOT_FOUND NULL
#define EMPTY_LIST -1
#define NO_PERSON_FOUND_INT -2
#define SINGLE_PERSON_IN_LIST -3
#define ERROR_ALLOC -4
#define ERROR_FILE -5

typedef struct Person* Position;

struct Person {
    char name[MAXSIZE];
    char surname[MAXSIZE];
    int birthYear;
    Position next;
};

typedef struct Person Person;

// Function prototypes
Position createPerson();
int addOnBeg(Position P);
int addOnEnd(Position P);
int printPerson(Position P);
int printList(Position P);
int deletePerson(Position P);
Position findPerson(Position P, const char* surname);
Position findLast(Position P);
int insertAfter(Position P);
int insertBefore(Position P);
int readFile(Position P);
int typeInFile(Position P);
int sortList(Position P);

// Main program/menu
int main() {
    Person Head = { .next = NULL, .name = { 0 }, .surname = { 0 }, .birthYear = 0 };
    Menu(&Head);
    return 0;
}

// Function definitions

Position createPerson() {
    Position newPerson = NULL;
    char name[MAXSIZE] = { 0 };
    char surname[MAXSIZE] = { 0 };
    int birthYear = 0;

    newPerson = (Position)malloc(sizeof(Person));
    if (newPerson == NULL) {
        printf("Error during allocation\n");
        return NULL;
    }

    printf("Enter name: ");
    scanf(" %s", name);
    printf("Enter surname: ");
    scanf(" %s", surname);
    printf("Enter year of birth: ");
    scanf(" %d", &birthYear);

    strcpy(newPerson->name, name);
    strcpy(newPerson->surname, surname);
    newPerson->birthYear = birthYear;
    newPerson->next = NULL;

    return newPerson;
}

int printPerson(Position P) {
    printf("\nName: %s, Surname: %s, BirthYear: %d", P->name, P->surname, P->birthYear);
    return SUCCESS;
}

int addOnBeg(Position P) {
    Position newPerson = createPerson();

    if (newPerson != NULL) {
        newPerson->next = P->next;
        P->next = newPerson;
        return SUCCESS;
    }

    printf("Failed to add a person to the beginning of the list!\n");
    return ERROR_ALLOC;
}

int addOnEnd(Position P) {
    Position newPerson = createPerson();
    if (newPerson != NULL) {
        Position lastPerson = findLast(P);
        lastPerson->next = newPerson;
        return SUCCESS;
    }

    printf("Failed to add a person to the end of the list!\n");
    return ERROR_ALLOC;
}

Position findLast(Position P) {
    Position current = P;
    while (current->next) {
        current = current->next;
    }
    return current;
}

int printList(Position P) {
    Position current = P->next;

    if (current == NULL) {
        printf("The list is empty.\n");
        return EMPTY_LIST;
    }

    while (current != NULL) {
        printPerson(current);
        current = current->next;
    }

    return SUCCESS;
}

Position findPerson(Position P, const char* surname) {
    Position current = P->next;

    while (current != NULL) {
        if (strcmp(current->surname, surname) == 0) {
            return current;
        }
        current = current->next;
    }

    return NULL;
}

int deletePerson(Position P) {
    char surname[MAXSIZE] = { 0 };
    printf("Enter surname of the person you want to delete: ");
    scanf(" %s", surname);

    Position current = P;
    Position previous = NULL;

    while (current->next != NULL && strcmp(surname, current->surname) != 0) {
        previous = current;
        current = current->next;
    }

    if (current != NULL && strcmp(current->surname, surname) == 0) {
        if (previous != NULL) {
            previous->next = current->next;
        }
        free(current);
        return SUCCESS;
    }

    printf("Person with surname %s not found in the list!\n", surname);
    return NO_PERSON_FOUND_INT;
}

int insertAfter(Position P) {
    char surname[MAXSIZE];
    printf("Enter the surname you want to insert after: ");
    scanf(" %s", surname);
    Position person = findPerson(P, surname);
    if (person != NULL) {
        Position newPerson = createPerson();
        if (newPerson != NULL) {
            newPerson->next = person->next;
            person->next = newPerson;
            return SUCCESS;
        }
        else {
            printf("Failed to create a new person.\n");
            return ERROR_ALLOC;
        }
    }
    else {
        printf("Can't find a person with the given surname to insert after.\n");
        return NOT_FOUND;
    }
}

int insertBefore(Position P) {
    char surname[MAXSIZE];
    printf("Enter the surname you want to insert before: ");
    scanf(" %s", surname);

    Position current = P->next;
    Position previous = P;

    while (current != NULL && strcmp(current->surname, surname) != 0) {
        previous = current;
        current = current->next;
    }

    if (current != NULL) {
        Position newPerson = createPerson();
        if (newPerson != NULL) {
            newPerson->next = current;
            previous->next = newPerson;
            return SUCCESS;
        }
        else {
            printf("Failed to create a new person.\n");
            return ERROR_ALLOC;
        }
    }
    else {
        printf("Can't find a person with the given surname to insert before.\n");
        return NOT_FOUND;
    }
}

int readFile(Position P) {
    FILE* filepointer = NULL;

    filepointer = fopen("ljudi.txt", "r");

    if (filepointer == NULL) {
        printf("Error in file opening.\n");
        return ERROR_FILE;
    }
    else {
        printf("File opened!\n");
    }

    while (!feof(filepointer)) {
        Position newPerson = NULL;
        Position last = NULL;

        char name[MAXSIZE] = { 0 };
        char surname[MAXSIZE] = { 0 };
        int birthYear = 0;

        newPerson = (Position)malloc(sizeof(Person));

        if (newPerson == NULL) {
            printf("Allocation error.\n");
            return ERROR_ALLOC;
        }

        fscanf(filepointer, "%s %s %d", name, surname, &birthYear);
        strcpy(newPerson->name, name);
        strcpy(newPerson->surname, surname);
        newPerson->birthYear = birthYear;

        if (newPerson != NULL) {
            last = findLast(P);
            newPerson->next = last->next;
            last->next = newPerson;
        }
    }

    fclose(filepointer);

    return SUCCESS;
}

int typeInFile(Position P) {
    Position current = P;
    FILE* filepointer = NULL;

    filepointer = fopen("data.txt", "w");

    if (filepointer == NULL) {
        printf("Error in file opening.\n");
        return ERROR_FILE;
    }
    else {
        printf("File opened!\n");
    }

    if (current == NULL) {
        printf("There is no person in the list.\n");
        return NO_PERSON_FOUND_INT;
    }

    while (current != NULL) {
        fprintf(filepointer, "Name: %s\nSurname: %s\nBirth year: %d\n", current->name, current->surname, current->birthYear);
        current = current->next;
    }
    fclose(filepointer);

    return SUCCESS;
}

int Menu(Position Head) {
    char option;
    while (1) {
        printf("MENU:\n");
        printf("Options:\n");
        printf("A: Add to the top of the list\n");
        printf("B: Add to the end of the list\n");
        printf("C: Find person by surname\n");
        printf("D: Delete person from the list\n");
        printf("E: Print list\n");
        printf("F: Insert after a person\n");
        printf("G: Insert before a person\n");
        printf("H: Sort list\n");
        printf("I: Insert list in file\n");
        printf("J: Read list from file\n");
        printf("K: Exit\n");
        scanf(" %c", &option);

        switch (option) {
        case 'A':
            addOnBeg(Head);
            break;
        case 'B':
            addOnEnd(Head);
            break;
        case 'C':
            printf("Enter the surname you want to find: ");
            char surname[MAXSIZE];
            scanf(" %s", surname);
            Position found = findPerson(Head, surname);
            if (found != NULL) {
                printf("Found person:\n");
                printPerson(found);
            }
            else {
                printf("Person with surname %s not found in the list!\n", surname);
            }
            break;
        case 'D':
            deletePerson(Head);
            break;
        case 'E':
            printList(Head);
            break;
        case 'F':
            insertAfter(Head);
            break;
        case 'G':
            insertBefore(Head);
            break;
        case 'H':
            break;
        case 'I':
            switch (readFile(Head)) {
            case SUCCESS:
                printf("File read successfully.\n");
                break;
            case ERROR_FILE:
                break;
            case ERROR_ALLOC:
                break;
            }
            break;
        case 'J':
            typeInFile(Head->next);
            break;
        case 'K':
            return SUCCESS;
        default:
            printf("Wrong letter!\n");
            break;
        }
    }
}