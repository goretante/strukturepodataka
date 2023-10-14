#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>

#define MAX_SIZE (50)
#define MAX_LINE (1024)
#define FILE_ERROR_OPEN (-1)

typedef struct _student {
    char name[MAX_SIZE];
    char surname[MAX_SIZE];
    double points;
} Student;

int brojacRedova(const char* imefilea) {
    FILE* filePointer = NULL;
    char buffer[MAX_LINE] = { 0 };
    int counter = 0;

    filePointer = fopen(imefilea, "r");

    if (!filePointer) {
        perror("Error opening file");
        return FILE_ERROR_OPEN;
    }

    while (fgets(buffer, MAX_LINE, filePointer) != NULL) {
        counter++;
    }

    fclose(filePointer);

    return counter;
}

int main() {
    int noRows = 0;

    noRows = brojacRedova("studenti.txt");

    printf("%d\n", noRows);

    return 0;
}