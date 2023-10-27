/*
3. Prethodnom zadatku dodati funkcije:
	A. dinamièki dodaje novi element iza odreðenog elementa, - solved
	B. dinamièki dodaje novi element ispred odreðenog elementa, - solved
	C. sortira listu po prezimenima osoba, - solved
	D. upisuje listu u datoteku,
	E. èita listu iz datoteke.
*/

#define _CRT_SECURE_NO_WARNINGS

// includes
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

// defines
#define MAXSIZE					50
#define SUCCESS					0
#define NOT_FOUND				NULL
#define EMPTY_LIST				-1
#define NO_PERSON_FOUND_INT		-2
#define SINGLE_PERSON_IN_LIST	-3
#define ERROR_ALLOC				-4


// structures
typedef struct Osoba* Position;

struct Osoba {
	char ime[MAXSIZE];
	char prezime[MAXSIZE];
	int godinaRodenja;
	Position next;
};

typedef struct Osoba Osoba;

// function prototypes
Position CreatePerson();
int AddOnBeginning(Position P);
int PrintPerson(Position P);
int PrintList(Position P);
Position FindLast(Position P);
int AddOnEnd(Position P);
int Menu(Position Head);
char* enterSurname();
Position FindPerson(Position firstItem);
Position FindPrevious(Position P);
int DeletePerson(Position P);
int swapItems(Position first, Position second);
int Sort(Position firstItem);


// main
int main() {

	Osoba Head = { .next = NULL, .ime = {0}, .prezime = {0}, .godinaRodenja = 0 };

	Menu(&Head);

	return 0;
}


// functions
int Menu(Position Head) {
	char choice = "\0";
	Position person = NULL;
	Position previous = NULL;
	while (1) {
		printf("MENU:\n");
		printf("Enter: A(dd to the front of the list)\n\tE(nd of the list add)\n\tI(nsert after person)\n\t(Insert) B(efore person)\n\tF(ind person)\n\tS(ort)\n\tD(elete)\n\tP(rint list)\n\t(e)X(it program)\n");
		scanf(" %c", &choice);
		switch (choice) {
		case 'A':
		case 'a':
			AddOnBeginning(Head);
			continue;
		case 'E':
		case 'e':
			AddOnEnd(Head);
			continue;
		case 'F':
		case 'f':
			person = FindPerson(Head->next);
			person ? PrintPerson(person) : printf("Can't find person with that surname!\n");
			continue;
		case 'D':
		case 'd':
			switch (DeletePerson(Head)) {
			case SUCCESS:
				printf("Deleted!\n");
				break;
			case EMPTY_LIST:
				printf("List is empty!\n");
				break;
			case NO_PERSON_FOUND_INT:
				printf("Can't find person with that surname!\n");
				break;
			}
			continue;
		case 'P':
		case 'p':
			PrintList(Head->next);
			continue;
		case 'I':
		case 'i':
			person = FindPerson(Head->next);
			person ? InsertAfterPerson(person) : printf("Can't find that surname.\n");
			continue;
		case 'B':
		case 'b':
			previous = FindPrevious(Head);
			previous ? InsertAfterPerson(previous) : printf("Can't find that surname.\n");
			continue;
		case 'S':
		case 's':
			switch (Sort(Head->next)) {
			case SUCCESS:
				printf("Sorted!\n");
				PrintList(Head->next);
				break;
			case EMPTY_LIST:
				printf("List is empty!\n");
				break;
			case SINGLE_PERSON_IN_LIST:
				printf("No use: single person in list.\n");
				break;
			default:
				break;
			}
			continue;
		case 'X':
		case 'x':
			break;
		default:
			printf("Wrong letter!\n");
			continue;
		}
		break;
	}

	return SUCCESS;
}

Position FindPrevious(Position P) {
	Position current = P;
	char surname[MAXSIZE] = { 0 };

	if (P->next == NULL) {
		printf("Empty list!\n");
		return NOT_FOUND;
	}

	strcpy(surname, enterSurname());

	do {
		if (strcmp(surname, current->next->prezime) == 0) {
			return current;
		}
		else {
			current = current->next;
		}
	} while (current->next != NULL);

	return NOT_FOUND;

}

int InsertAfterPerson(Position P) {
	Position newPerson = NULL;

	newPerson = CreatePerson();

	if (newPerson != NULL) {
		newPerson->next = P->next;
		P->next = newPerson;
	}

	return SUCCESS;
}


int DeletePerson(Position P) {
	Position current = P;
	char surname[MAXSIZE] = { 0 };

	strcpy(surname, enterSurname());

	if (P->next != NULL) {
		Position previous = NULL;

		while (current->next != NULL && strcmp(surname, current->prezime) != 0) {
			previous = current;
			current = current->next;
		}

		if (previous != NULL && previous->next != NULL && strcmp(current->prezime, surname) == 0) {
			PrintPerson(current);
			previous->next = current->next;
			free(current);
		}
		else {
			return NO_PERSON_FOUND_INT;
		}
	}
	else {
		return EMPTY_LIST;
	}

	return SUCCESS;
}

int swapItems(Position first, Position second) {
	char tempName[MAXSIZE];
	char tempSurname[MAXSIZE];
	int tempBirthYear;

	strcpy(tempName, first->ime);
	strcpy(tempSurname, first->prezime);
	tempBirthYear = first->godinaRodenja;

	strcpy(first->ime, second->ime);
	strcpy(first->prezime, second->prezime);
	first->godinaRodenja = second->godinaRodenja;

	strcpy(second->ime, tempName);
	strcpy(second->prezime, tempSurname);
	second->godinaRodenja = tempBirthYear;

	return SUCCESS;
}



int Sort(Position firstItem) {
	int swapped = 0;
	Position lastPersonRead = NULL;
	Position start = firstItem;

	if (firstItem == NULL) {
		printf("Empty List!\n");
		return NO_PERSON_FOUND_INT;
	}
	else if (firstItem->next == NULL) {
		printf("Only single element in list!\n");
		return SINGLE_PERSON_IN_LIST;
	}

	do {
		swapped = 0;
		Position current = start;

		while (current->next != lastPersonRead) {
			if (strcmp(current->prezime, current->next->prezime) > 0) {
				swapItems(current, current->next);
				swapped = 1;
			}
		}
		lastPersonRead = current;
	} while (!swapped);

	return SUCCESS;
}

char* enterSurname() {
	char surname[MAXSIZE] = { 0 };
	printf("Enter surname of the wanted person: \n");
	scanf(" %s", surname);

	return surname;
}

Position FindPerson(Position firstItem) {
	Position current = firstItem;
	char surname[MAXSIZE] = { 0 };

	if (firstItem == NULL) {
		printf("List is empty!");
		return NOT_FOUND;
	}

	strcpy(surname, enterSurname());

	do {
		if (strcmp(current->prezime, surname) == 0) {
			return current;
		}
		else {
			current = current->next;
		}
	} while (current != NULL);

	return NOT_FOUND;
}

Position FindLast(Position P) {
	Position current = P;

	while (current->next) {
		current = current->next;
	}

	return current;
}

int AddOnEnd(Position P) {
	Position newPerson = NULL;
	Position last = NULL;

	newPerson = CreatePerson();

	if (newPerson != NULL) {
		last = FindLast(P);
		newPerson->next = last->next;
		last->next = newPerson;
	}

	return SUCCESS;
}

int PrintList(Position firstItem) {

	Position current = firstItem;

	if (firstItem == NULL) {
		printf("The list is empty.\n");
	}

	while (current != NULL) {
		PrintPerson(current);
		current = current->next;
	}

	return SUCCESS;
}

int AddOnBeginning(Position P) {

	Position newPerson;

	newPerson = CreatePerson();

	if (newPerson != NULL) {
		newPerson->next = P->next;
		P->next = newPerson;
	}

	printf("The person is added on the beginning of the list!\n");

	return SUCCESS;
}

Position CreatePerson() {
	Position newPerson = NULL;
	char name[MAXSIZE] = { 0 };
	char surname[MAXSIZE] = { 0 };
	int birthYear = 0;

	newPerson = (Position)malloc(sizeof(Osoba));
	if (newPerson == NULL) {
		printf("Allocation error.\n");
		return ERROR_ALLOC;
	}

	printf("Enter name: ");
	scanf(" %s", name);
	printf("Enter surname: ");
	scanf(" %s", surname);
	printf("Enter year of birth: ");
	scanf(" %d", &birthYear);

	strcpy(newPerson->ime, name);
	strcpy(newPerson->prezime, surname);
	newPerson->godinaRodenja = birthYear;

	return newPerson;
}

int PrintPerson(Position P) {
	printf("Name: %s\t Surname: %s\t Birth year: %d\t\n", P->ime, P->prezime, P->godinaRodenja);
	return SUCCESS;
}