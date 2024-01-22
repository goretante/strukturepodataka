#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define _CRT_SECURE_NO_WARNINGS

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

// Struktura za èvor vezane liste država
typedef struct ListaDrzava {
    Drzava* drzava;
    struct ListaDrzava* next;
} ListaDrzava;

// Hash tablica
#define VELICINA_TABLICE 11

// Funkcija za preslikavanje kljuèa
int hash_funkcija(char naziv[]) {
    int suma = 0;
    for (int i = 0; i < 5 && naziv[i] != '\0'; i++) {
        suma += (int)naziv[i];
    }
    return suma % VELICINA_TABLICE;
}

// Funkcija za dodavanje države u vezanu listu sortiranu po nazivu
ListaDrzava* dodaj_u_listu(ListaDrzava* head, Drzava* nova_drzava) {
    ListaDrzava* nova_lista_drzava = (ListaDrzava*)malloc(sizeof(ListaDrzava));
    nova_lista_drzava->drzava = nova_drzava;
    nova_lista_drzava->next = NULL;

    if (head == NULL || strcmp(nova_drzava->naziv, head->drzava->naziv) < 0) {
        nova_lista_drzava->next = head;
        return nova_lista_drzava;
    }

    ListaDrzava* trenutna = head;
    while (trenutna->next != NULL && strcmp(nova_drzava->naziv, trenutna->next->drzava->naziv) > 0) {
        trenutna = trenutna->next;
    }

    nova_lista_drzava->next = trenutna->next;
    trenutna->next = nova_lista_drzava;

    return head;
}

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

// Funkcija za pretragu gradova odreðene države s brojem stanovnika veæim od zadane vrijednosti
void pretraga_gradova(Drzava* drzava, int min_broj_stanovnika) {
    printf("Gradovi u %s sa brojem stanovnika vecim od %d:\n", drzava->naziv, min_broj_stanovnika);
    ispisi_gradove(drzava->gradovi);
}

int main() {
    // Inicijalizacija hash tablice
    ListaDrzava* hash_tablica[VELICINA_TABLICE];
    for (int i = 0; i < VELICINA_TABLICE; i++) {
        hash_tablica[i] = NULL;
    }

    // Èitanje datoteke drzave.txt
    FILE* datoteka_drzave = fopen("drzave.txt", "r");
    if (datoteka_drzave == NULL) {
        perror("Greska pri otvaranju datoteke drzave.txt");
        exit(EXIT_FAILURE);
    }

    char naziv_drzave[50];
    char datoteka_gradovi[50];
    while (fscanf(datoteka_drzave, "%s %s", naziv_drzave, datoteka_gradovi) == 2) {
        // Dodavanje države u hash tablicu
        int kljuc = hash_funkcija(naziv_drzave);
        Drzava* nova_drzava = (Drzava*)malloc(sizeof(Drzava));
        strcpy(nova_drzava->naziv, naziv_drzave);
        strcpy(nova_drzava->datoteka_gradovi, datoteka_gradovi);
        nova_drzava->gradovi = NULL;
        nova_drzava->next = NULL;

        hash_tablica[kljuc] = dodaj_u_listu(hash_tablica[kljuc], nova_drzava);

        // Èitanje datoteke s gradovima
        FILE* datoteka_gradovi = fopen(datoteka_gradovi, "r");
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
        nova_drzava->gradovi = stablo_gradova;

        fclose(datoteka_gradovi);
    }

    // Zatvaranje datoteke drzave.txt
    fclose(datoteka_drzave);

    // Ispis država i gradova
    for (int i = 0; i < VELICINA_TABLICE; i++) {
        ListaDrzava* trenutna_lista = hash_tablica[i];
        while (trenutna_lista != NULL) {
            printf("Drzava: %s\n", trenutna_lista->drzava->naziv);
            printf("Gradovi:\n");
            ispisi_gradove(trenutna_lista->drzava->gradovi);
            printf("\n");
            trenutna_lista = trenutna_lista->next;
        }
    }

    // Pretraga gradova odreðene države
    char trazena_drzava[50];
    int min_broj_stanovnika;

    printf("Unesite ime drzave za pretragu: ");
    scanf("%s", trazena_drzava);

    printf("Unesite minimalan broj stanovnika: ");
    scanf("%d", &min_broj_stanovnika);

    int kljuc_pretrage = hash_funkcija(trazena_drzava);
    ListaDrzava* trenutna_lista = hash_tablica[kljuc_pretrage];
    while (trenutna_lista != NULL) {
        if (strcmp(trenutna_lista->drzava->naziv, trazena_drzava) == 0) {
            pretraga_gradova(trenutna_lista->drzava, min_broj_stanovnika);
            break;
        }
        trenutna_lista = trenutna_lista->next;
    }

    // Oslobaðanje memorije
    for (int i = 0; i < VELICINA_TABLICE; i++) {
        while (hash_tablica[i] != NULL) {
            ListaDrzava* temp_lista = hash_tablica[i];
            hash_tablica[i] = hash_tablica[i]->next;
            free(temp_lista->drzava);
            free(temp_lista);
        }
    }

    return 0;
}