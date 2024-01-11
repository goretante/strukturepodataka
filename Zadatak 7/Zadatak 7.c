#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct Directory {
    char name[50];
    struct Directory* subdirectories;
    struct Directory* parent;
};

struct Directory* createDirectory(char name[]);
int addSubdirectory(struct Directory* parent, char name[]);
int printDirectory(struct Directory* dir);
struct Directory* navigateUp(struct Directory* currentDir);

int main() {
    struct Directory* root = createDirectory("C:");
    struct Directory* currentDir = root;

    int choice;
    char dirname[50];

    do {
        printf("\nDOS Simulator Menu:\n");
        printf("1 - md (Create directory)\n");
        printf("2 - cd dir (Change directory)\n");
        printf("3 - cd.. (Navigate up)\n");
        printf("4 - dir (List contents)\n");
        printf("5 - Exit\n");

        printf("Enter your choice: ");
        if (scanf("%d", &choice) != 1) {
            printf("Invalid input. Please enter a number.\n");
            while (getchar() != '\n');
            continue;
        }

        switch (choice) {
        case 1:
            printf("Enter directory name: ");
            if (scanf("%s", dirname) != 1) {
                printf("Invalid directory name.\n");
                while (getchar() != '\n'); 
                break;
            }
            if (addSubdirectory(currentDir, dirname) == EXIT_SUCCESS) {
                printf("Directory created successfully.\n");
            }
            else {
                printf("Failed to create directory.\n");
            }
            break;
        case 2:
            printf("Enter directory name: ");
            if (scanf("%s", dirname) != 1) {
                printf("Invalid directory name.\n");
                while (getchar() != '\n');
                break;
            }
            struct Directory* tempDir = currentDir->subdirectories;
            while (tempDir != NULL) {
                if (strcmp(tempDir->name, dirname) == 0) {
                    currentDir = tempDir;
                    break;
                }
                tempDir = tempDir->subdirectories;
            }
            if (tempDir == NULL) {
                printf("Directory not found.\n");
            }
            break;
        case 3:
            currentDir = navigateUp(currentDir);
            break;
        case 4:
            if (printDirectory(currentDir) == EXIT_FAILURE) {
                printf("This directory is empty.\n");
            }
            break;
        case 5:
            printf("Exiting program.\n");
            break;
        default:
            printf("Invalid choice. Please enter a number between 1 and 5.\n");
        }
    } while (choice != 5);

    free(root);

    return 0;
}

struct Directory* createDirectory(char name[]) {
    struct Directory* newDir = (struct Directory*)malloc(sizeof(struct Directory));
    if (newDir == NULL) {
        printf("Memory allocation failed.\n");
        exit(EXIT_FAILURE);
    }
    strcpy(newDir->name, name);
    newDir->subdirectories = NULL;
    newDir->parent = NULL;
    return newDir;
}

int addSubdirectory(struct Directory* parent, char name[]) {
    struct Directory* newSubdir = createDirectory(name);
    if (newSubdir == NULL) {
        return EXIT_FAILURE;
    }

    newSubdir->parent = parent;
    newSubdir->subdirectories = parent->subdirectories;
    parent->subdirectories = newSubdir;
    return EXIT_SUCCESS;
}

int printDirectory(struct Directory* dir) {
    printf("Contents of directory %s:\n", dir->name);

    struct Directory* current = dir->subdirectories;
    int count = 0;

    while (current != NULL) {
        printf("- %s\n", current->name);
        current = current->subdirectories;
        count++;
    }

    return (count > 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}

struct Directory* navigateUp(struct Directory* currentDir) {
    if (currentDir->parent != NULL) {
        return currentDir->parent;
    }
    else {
        printf("You are already in the root directory.\n");
        return currentDir;
    }
}