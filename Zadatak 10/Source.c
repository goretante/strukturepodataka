#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Struktura za grad
typedef struct Grad {
    char naziv[50];
    int broj_stanovnika;
    struct Grad* left;
    struct Grad* right;
} Grad;

// Struktura za državu
typedef struct Drzava {
    char naziv[50];
    char datoteka_gradovi[50];
    Grad* gradovi;
    struct Drzava* next;
} Drzava;

// Funkcija za dodavanje grada u stablo
Grad* dodaj_grad(Grad* root, char naziv[], int broj_stanovnika) {
    if (root == NULL) {
        Grad* novi_grad = (Grad*)malloc(sizeof(Grad));
        strcpy(novi_grad->naziv, naziv);
        novi_grad->broj_stanovnika = broj_stanovnika;
        novi_grad->left = NULL;
        novi_grad->right = NULL;
        return novi_grad;
    }

    if (broj_stanovnika < root->broj_stanovnika) {
        root->left = dodaj_grad(root->left, naziv, broj_stanovnika);
    }
    else if (broj_stanovnika >= root->broj_stanovnika) {
        root->right = dodaj_grad(root->right, naziv, broj_stanovnika);
    }

    return root;
}

// Funkcija za ispisivanje gradova u inorder poredak
void ispisi_gradove(Grad* root) {
    if (root != NULL) {
        ispisi_gradove(root->left);
        printf("%s, %d\n", root->naziv, root->broj_stanovnika);
        ispisi_gradove(root->right);
    }
}

// Funkcija za dodavanje države u vezanu listu
Drzava* dodaj_drzavu(Drzava* head, char naziv[], char datoteka_gradovi[]) {
    Drzava* nova_drzava = (Drzava*)malloc(sizeof(Drzava));
    strcpy(nova_drzava->naziv, naziv);
    strcpy(nova_drzava->datoteka_gradovi, datoteka_gradovi);
    nova_drzava->gradovi = NULL;
    nova_drzava->next = head;
    return nova_drzava;
}

// Funkcija za pretragu gradova odreðene države s brojem stanovnika veæim od zadane vrijednosti
void pretraga_gradova(Drzava* head, char trazena_drzava[], int min_broj_stanovnika) {
    Drzava* trenutna_drzava = head;

    while (trenutna_drzava != NULL) {
        if (strcmp(trenutna_drzava->naziv, trazena_drzava) == 0) {
            printf("Gradovi u %s sa brojem stanovnika vecim od %d:\n", trazena_drzava, min_broj_stanovnika);
            ispisi_gradove(trenutna_drzava->gradovi);
            return;
        }
        trenutna_drzava = trenutna_drzava->next;
    }

    printf("Drzava %s nije pronadena.\n", trazena_drzava);
}

int main() {
    // Inicijalizacija
    Drzava* lista_drzava = NULL;

    // Èitanje datoteke drzave.txt
    FILE* datoteka_drzave = fopen("drzave.txt", "r");
    if (datoteka_drzave == NULL) {
        perror("Greska pri otvaranju datoteke drzave.txt");
        exit(EXIT_FAILURE);
    }

    char naziv_drzave[50];
    char datoteka_gradova[50];
    while (fscanf(datoteka_drzave, "%s %s", naziv_drzave, datoteka_gradova) == 2) {
        // Dodavanje države u vezanu listu
        lista_drzava = dodaj_drzavu(lista_drzava, naziv_drzave, datoteka_gradova);

        // Èitanje datoteke s gradovima
        FILE* datoteka_gradovi = fopen(datoteka_gradova, "r");
        if (datoteka_gradovi == NULL) {
            perror("Greska pri otvaranju datoteke s gradovima");
            exit(EXIT_FAILURE);
        }

        char naziv_grada[50];
        int broj_stanovnika;
        Grad* stablo_gradova = NULL;

        // Èitanje gradova iz datoteke i dodavanje u stablo
        while (fscanf(datoteka_gradovi, "%s %d", naziv_grada, &broj_stanovnika) == 2) {
            stablo_gradova = dodaj_grad(stablo_gradova, naziv_grada, broj_stanovnika);
        }

        // Povezivanje stabla gradova s odgovarajuæom državom
        lista_drzava->gradovi = stablo_gradova;

        fclose(datoteka_gradovi);
    }

    // Zatvaranje datoteke drzave.txt
    fclose(datoteka_drzave);

    // Ispis država i gradova
    Drzava* trenutna_drzava = lista_drzava;
    while (trenutna_drzava != NULL) {
        printf("Drzava: %s\n", trenutna_drzava->naziv);
        printf("Gradovi:\n");
        ispisi_gradove(trenutna_drzava->gradovi);
        printf("\n");

        trenutna_drzava = trenutna_drzava->next;
    }

    // Pretraga gradova odreðene države
    char trazena_drzava[50];
    int min_broj_stanovnika;

    printf("Unesite ime drzave za pretragu: ");
    scanf("%s", trazena_drzava);

    printf("Unesite minimalan broj stanovnika: ");
    scanf("%d", &min_broj_stanovnika);

    pretraga_gradova(lista_drzava, trazena_drzava, min_broj_stanovnika);

    // Oslobaðanje memorije
    while (lista_drzava != NULL) {
        Drzava* temp_drzava = lista_drzava;
        lista_drzava = lista_drzava->next;
        free(temp_drzava);
    }

    return 0;
}