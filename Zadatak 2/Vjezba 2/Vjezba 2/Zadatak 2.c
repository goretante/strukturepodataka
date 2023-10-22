// micanje warninga
#define _CRT_SECURE_NO_WARNINGS

// includeovi
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// defineovi
#define MAXSIZE			50
#define USPJEH			0
#define NIJE_PRONADENO	0
#define ALLOC_GRESKA	-1


// definiranje struktura
typedef struct Osoba* Pozicija;

typedef struct Osoba{
	char ime[MAXSIZE];
	char prezime[MAXSIZE];
	int godinaRodenja;
	Pozicija next;
};

typedef struct Osoba Osoba;

// funkcije
Pozicija DodajHead();
int DodajNaPocetak(Pozicija Head);
int IspisListe(Pozicija Head);
int DodajNaKraj(Pozicija Head);
int PronadiElement(Pozicija Head);
int PronadiPrethodni(Pozicija Head);
int Brisanje(Pozicija Prethodni);

int main() {

	Pozicija Head = NULL;
	Head = DodajHead();
	int i;

	if (Head == NULL) {
		printf("Alokacija heada neuspjesna!\n");
		return ALLOC_GRESKA;
	}
	else {
		printf("Head je stvoren!\n");
	}


	for (i = 0; i < 2; i++) {
		DodajNaPocetak(Head);
	}

	for (i = 0; i < 2; i++) {
		DodajNaKraj(Head);
	}

	IspisListe(Head);

	Pozicija trazEl = NULL;
	Pozicija pretEl = NULL;

	trazEl = PronadiElement(Head);
	pretEl = PronadiPrethodni(Head);

	Brisanje(pretEl);

	IspisListe(Head);

	return 0;

}

Pozicija DodajHead() {
	Pozicija head = NULL;
	head = (Pozicija)malloc(sizeof(Osoba));
	head->next = NULL;
	return head;
}

int DodajNaPocetak(Pozicija Head) {

	Pozicija p = NULL;
	p = Head;

	Pozicija q = NULL;

	q = (Pozicija)malloc(sizeof(Osoba));

	if (q == NULL) {
		printf("Neuspjesno dodavanje novog elementa u listu.\n");
		return ALLOC_GRESKA;
	}
	else {
		printf("Dodan novi element u listu.\n");
	}

	q->next = p->next;
	p->next = q;

	printf("Unesi ime: ");
	scanf(" %s", q->ime);
	printf("Unesi prezime: ");
	scanf(" %s", q->prezime);
	printf("Unesi godinu rodenja: ");
	scanf(" %d", &(q->godinaRodenja));

	printf("Osoba je dodana na pocetak liste!\n");
	
	return USPJEH;

}

int IspisListe(Pozicija Head) {
	
	Pozicija temp;
	temp = Head->next;

	int brojac = 1;

	while (temp = NULL) {
		printf("%d. osoba:\n");
		printf("Ime: %s\n", temp->ime);
		printf("Prezime: %s\n", temp->prezime);
		printf("Godina rodenja: %d.\n", temp->godinaRodenja);
		temp = temp->next;
		brojac++;
	}

	return USPJEH;

}

int DodajNaKraj(Pozicija Head) {
	
	Pozicija p = NULL;
	p = Head;

	while (p->next != NULL) {
		p = p->next;
	}

	Pozicija q = NULL;

	q = (Pozicija)malloc(sizeof(Osoba));
	if (q == NULL) {
		printf("Neuspjesno dodavanje novog elementa u listu.\n");
		return ALLOC_GRESKA;
	}
	else {
		printf("Dodan novi element u listu.\n");
	}

	q->next = p->next;
	p->next = q;

	printf("Unesi ime: ");
	scanf(" %s", q->ime);
	printf("Unesi prezime: ");
	scanf(" %s", q->prezime);
	printf("Unesi godinu rodenja: ");
	scanf(" %d", &(q->godinaRodenja));

	printf("Osoba je dodana na kraj liste!\n");

	return USPJEH;

}

int PronadiElement(Pozicija Head) {

	Pozicija p = Head;

	char tempPrezime[MAXSIZE];

	printf("Unesi prezime koje zelis pretraziti: \n");
	scanf(" %s", tempPrezime);

	int brojac = 1;

	while (p->next != NULL && strcmp(tempPrezime, p->next->prezime) != 0) {
		p = p->next;
		brojac++;
	}

	if (strcmp(tempPrezime, p->next->prezime) == 0) {
		printf("Trazeno prezime je pronadeno na %d. mjestu.\n", brojac);
		return p->next;
	}
	else {
		printf("Prezime nije pronadeno.\n");
		return NIJE_PRONADENO;
	}

}

int PronadiPrethodni(Pozicija Head) {

	Pozicija p = Head;

	char tempPrezime[MAXSIZE];
	
	printf("Unesi prezime koje zelis pretraziti: ");
	scanf(" %s", tempPrezime);

	int brojac = 1;

	while (p->next != NULL && strcmp(tempPrezime, p->next->prezime) != 0) {
		p = p->next;
		brojac++;
	}

	if (strcmp(tempPrezime, p->next->prezime) == 0) {
		printf("Pronadeno odgovarajuce prezime na %d. poziciji, vracam prethodni.\n", brojac);
		return p;
	}
	else {
		printf("Nije pronadeno prezime.\n");
		return NIJE_PRONADENO;
	}

}

int Brisanje(Pozicija Prethodni) {
	
	Pozicija p = NULL;

	p = Prethodni->next;

	Prethodni->next = p->next;

	free(p);

	return USPJEH;

}