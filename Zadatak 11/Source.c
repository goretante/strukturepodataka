#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LINE 1024

typedef struct city {
	char city[MAX_LINE];
	int population;
	struct city* left;
	struct city* right;
}cities_;

typedef struct country {
	char country[MAX_LINE];
	struct country* next;
	cities_* cityhead;
}countries_;

typedef struct hash {
	countries_* table[11];
}hash_;

typedef hash_* hash;
typedef countries_* countries;
typedef cities_* cities;

int createhash(hash hashhead);
int readfile(countries head[]);
int rowscounter(char name[]);
int listmaker(countries head[], char buffername[], char bufferlink[]);
int treemaker(cities head, char bufferlink[]);
cities tree(cities head, char name[], int population);
int printall(countries table[]);
int print(countries head);
int townprint(cities head);

int main()
{
	hash_ hashhead;

	createhash(&hashhead.table);

	readfile(&hashhead);
	printall(hashhead.table);
}

int createhash(hash hashhead)
{
	for (int i = 0; i < 11; i++)
	{
		hashhead->table[i] = malloc(sizeof(countries_));
		hashhead->table[i]->next = NULL;
	}
}

int readfile(countries head[])
{
	char buffername[MAX_LINE];
	char bufferlink[MAX_LINE];
	int rows;

	rows = rowscounter("drzave.txt");
	FILE* fptr = fopen("drzave.txt", "r");
	if (fptr == NULL)
	{
		return -1;
	}

	for (int i = 0; i < rows; i++)
	{
		fscanf(fptr, "%s %s", buffername, bufferlink);
		listmaker(head, buffername, bufferlink);
	}

	fclose(fptr);
}

int rowscounter(char name[])
{
	int rownumber = 0;
	char buffer[MAX_LINE];

	FILE* fptr = fopen(name, "r");

	if (fptr == NULL)
	{
		return -1;
	}

	while (!feof(fptr))
	{
		fgets(buffer, MAX_LINE, fptr);
		rownumber++;
	}

	fclose(fptr);

	return rownumber;
}

int listmaker(countries head[], char buffername[], char bufferlink[])
{
	int ascii = 0;

	for (int i = 0; i < 5; i++)							//ako ima manje od 5 znakova svejedno ga broji
	{
		if (buffername[i] != '\0')
		{
			ascii += (int)buffername[i];
		}
		else
		{
			i = 5;
		}
	}

	ascii %= 11;

	countries ptr = malloc(sizeof(countries_));
	strcpy(ptr->country, buffername);

	ptr->cityhead = malloc(sizeof(cities_));
	ptr->cityhead->right = NULL;
	ptr->cityhead->left = NULL;

	treemaker(ptr->cityhead, bufferlink);

	ptr->next = head[ascii]->next;
	head[ascii]->next = ptr;
}

int treemaker(cities head, char bufferlink[])
{
	int rows;
	char buffername[MAX_LINE];
	int population;

	rows = rowscounter(bufferlink);

	FILE* fptr = fopen(bufferlink, "r");

	for (int i = 0; i < rows; i++)
	{
		fscanf(fptr, "%s %d", buffername, &population);
		head->right = tree(head->right, buffername, population);
	}
	return 0;
}

cities tree(cities head, char name[], int population)
{
	if (head == NULL)
	{
		cities ptr = malloc(sizeof(cities_));
		strcpy(ptr->city, name);
		ptr->population = population;
		ptr->left = NULL;
		ptr->right = NULL;
		return ptr;
	}
	else if (strcmp(head->city, name) >= 0)
	{
		head->left = tree(head->left, name, population);
	}
	else if (strcmp(head->city, name) < 0)
	{
		head->right = tree(head->right, name, population);
	}

	return head;
}

int printall(countries table[])
{
	for (int i = 0; i < 11; i++)
	{
		if (table[i]->next != NULL)
		{
			print(table[i]->next);
		}
	}
}

int print(countries head)
{
	while (head != NULL)
	{
		printf("%s\n", head->country);
		townprint(head->cityhead->right);
		head = head->next;
		printf("\n");
	}
}

int townprint(cities head)
{
	if (head == NULL)
	{
		return 0;
	}
	printf("%s %d\n", head->city, head->population);
	townprint(head->left);
	townprint(head->right);
}