/*
1. Napisati program koji prvo pročita koliko redaka ima datoteka, tj.koliko ima studenata
zapisanih u datoteci.Nakon toga potrebno je dinamički alocirati prostor za niz struktura
studenata(ime, prezime, bodovi) i učitati iz datoteke sve zapise.Na ekran ispisati ime,
prezime, apsolutni i relativni broj bodova.
Napomena: Svaki redak datoteke sadrži ime i prezime studenta, te broj bodova na kolokviju.
*/

#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAXNAME 128
#define MAXSIZE 1024
#define MAXPOINTS 15

typedef struct {
    char name[MAXNAME];
    char surname[MAXNAME];
    double points;
} Student;

int readNumberOfRows();
Student* readAndSaveStudents(int numStudents);
int printStudents(Student* students, int numOfStudents);

int main() {
    int numRows = 0;
    Student* students = NULL;

    numRows = readNumberOfRows();
    if (numRows > 0) {
        students = readAndSaveStudents(numRows);
        printStudents(students, numRows);

        free(students);
    }

    return EXIT_SUCCESS;
}

int readNumberOfRows() {
    int counter = 0;
    FILE* filePointer = NULL;
    char buffer[MAXNAME] = { 0 };

    filePointer = fopen("students.txt", "r");
    if (filePointer == NULL) {
        printf("\nCan't open file!");
        return -1;
    }


    while (!feof(filePointer)) {
        fgets(buffer, MAXNAME, filePointer);
        counter++;
    }

    fclose(filePointer);
    return counter;
}

Student* readAndSaveStudents(int numStudents) {

    Student* students = NULL;
    int counter = 0;
    FILE* filePointer = NULL;

    students = (Student*)malloc(numStudents * sizeof(Student));
    if (!students) {
        printf("Can't allocate memory!\n");
        return NULL;
    }

    filePointer = fopen("students.txt", "r");
    if (!filePointer) {
        printf("Can't open file!\n");
        return NULL;
    }

    while (!feof(filePointer)) {
        fscanf(filePointer, " %s %s %lf", students[counter].name, students[counter].surname, &students[counter].points);
        counter++;
    }

    fclose(filePointer);

    return students;
}

int printStudents(Student* students, int numOfStudents) {
    for (int counter = 0; counter < numOfStudents; counter++) {
        double relativePoints = students[counter].points / MAXPOINTS * 100;
        printf("Name: %s %s - Abs. points: %.1lf - Rel. points: %.2lf\n", students[counter].name, students[counter].surname, students[counter].points, relativePoints);
    }
    
    return EXIT_SUCCESS;
}