#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>

#define MAX_LINE 1024

typedef struct nebitno_ {
	char city[MAX_LINE];
	int population;
	struct nebitno_* next;
	struct nebitno_* left;
	struct nebitno_* right;
}cities_;

typedef struct nebitno {
	char country[MAX_LINE];
	cities_* cityhead;
	struct nebitno* next;
	struct nebitno* left;
	struct nebitno* right;
}countries_;

typedef cities_* cities;
typedef countries_* countries;

int readfilea(countries head);
int readfileb(countries head);
int rowscounter(char name[]);
int openfilea(countries head, int rows);
int openfileb(countries head, int rows);
int listinsert(countries head, char name[], char link[]);
int insertcity(cities head, char link[]);
cities cityinsert(cities head, char name[], int popluation);
int printa(countries head);
int printb(countries head);
int gradprinta(cities head);
int gradprintb(cities head);
countries treeinsert(countries head, char buffername[], char bufferlink[]);
int citylist(cities head, char link[]);
int listcity(cities head, char name[], int population);
int lobby(countries head);
int treesearch(cities head, int pop);

int main()
{
	countries_ heada;
	countries_ headb;
	heada.next = NULL;
	heada.right = NULL;
	heada.left = NULL;
	headb.next = NULL;
	headb.right = NULL;
	headb.left = NULL;

	readfilea(&heada);
	readfileb(&headb);

	printa(heada.next);

	printf("\n");

	printb(headb.next);

	lobby(heada.next);
}

int readfilea(countries head)
{
	int rownumber;
	rownumber = rowscounter("drzave.txt");

	openfilea(head, rownumber);
}

int readfileb(countries head)
{
	int rownumber;
	rownumber = rowscounter("drzave.txt");

	openfileb(head, rownumber);
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

int openfilea(countries head, int rows)
{
	char buffername[MAX_LINE];
	char bufferlink[MAX_LINE];

	FILE* fptr = fopen("drzave.txt", "r");

	for (int i = 0; i < rows; i++)
	{
		fscanf(fptr, "%s %s", buffername, bufferlink);

		if (i == 0)
		{
			strcpy(head->country, buffername);
			head->left = NULL;
			head->right = NULL;

			head->cityhead = malloc(sizeof(cities_));
			if (head->cityhead == NULL)
			{
				return -1;
			}

			head->cityhead->right = NULL;
			head->cityhead->left = NULL;
			head->cityhead->next = NULL;

			insertcity(head->cityhead, bufferlink);
		}
		listinsert(head, buffername, bufferlink);
	}

	fclose(fptr);
}

int openfileb(countries head, int rows)
{
	char buffername[MAX_LINE];
	char bufferlink[MAX_LINE];

	FILE* fptr = fopen("drzave.txt", "r");

	for (int i = 0; i < rows; i++)
	{
		fscanf(fptr, "%s %s", buffername, bufferlink);
		if (i == 0)
		{
			head->next = treeinsert(head, buffername, bufferlink);
		}
		else
		{
			treeinsert(head->next, buffername, bufferlink);
		}
	}

	fclose(fptr);
}

int listinsert(countries head, char name[], char link[])
{
	countries ptr = malloc(sizeof(countries_));
	strcpy(ptr->country, name);
	ptr->left = NULL;
	ptr->right = NULL;

	ptr->cityhead = malloc(sizeof(cities_));
	ptr->cityhead->right = NULL;
	ptr->cityhead->left = NULL;
	ptr->cityhead->next = NULL;

	insertcity(ptr->cityhead, link);

	while (head->next != NULL && strcmp(head->next->country, name) < 0)
	{
		head = head->next;
	}

	ptr->next = head->next;
	head->next = ptr;
}

int insertcity(cities head, char link[])
{
	char name[MAX_LINE];
	int population;
	int rows;
	FILE* fptr = fopen(link, "r");

	rows = rowscounter(link);

	for (int i = 0; i < rows; i++)
	{
		fscanf(fptr, "%s %d", name, &population);

		if (i == 0)
		{
			strcpy(head->city, name);
			head->population = population;
			head->left = NULL;
			head->right = NULL;
		}
		else
		{
			cityinsert(head, name, population);
		}
	}

	fclose(fptr);
}

cities cityinsert(cities head, char name[], int population)
{
	if (head == NULL)
	{
		cities ptr = malloc(sizeof(cities_));
		strcpy(ptr->city, name);
		ptr->population = population;
		ptr->left = NULL;
		ptr->right = NULL;
		ptr->next = NULL;
		return ptr;
	}
	else if (head->population > population)
	{
		head->left = cityinsert(head->left, name, population);
	}
	else if (head->population <= population)
	{
		head->right = cityinsert(head->right, name, population);
	}
}

countries treeinsert(countries head, char buffername[], char bufferlink[])
{
	if (head == NULL)
	{
		countries ptr = malloc(sizeof(countries_));
		if (ptr == NULL)
		{
			return -1;
		}

		strcpy(ptr->country, buffername);
		ptr->cityhead = malloc(sizeof(cities_));

		ptr->cityhead->left = NULL;
		ptr->cityhead->right = NULL;
		ptr->cityhead->next = NULL;

		ptr->left = NULL;
		ptr->right = NULL;
		ptr->next = NULL;
		citylist(ptr->cityhead, bufferlink);
		return ptr;
	}
	else if (strcmp(head->country, buffername) >= 0)
	{
		head->left = treeinsert(head->left, buffername, bufferlink);
	}
	else if (strcmp(head->country, buffername) < 0)
	{
		head->right = treeinsert(head->right, buffername, bufferlink);
	}
}

int citylist(cities head, char link[])
{
	char name[MAX_LINE];
	int population;
	int rows;

	FILE* fptr = fopen(link, "r");
	rows = rowscounter(link);

	for (int i = 0; i < rows; i++)
	{
		fscanf(fptr, "%s %d", name, &population);
		listcity(head, name, population);
	}

	fclose(fptr);
}

int listcity(cities head, char name[], int population)
{
	cities ptr = malloc(sizeof(cities_));
	if (ptr == NULL)
	{
		return -1;
	}

	strcpy(ptr->city, name);
	ptr->population = population;
	ptr->left = NULL;
	ptr->right = NULL;

	while (head->next != NULL && strcmp(head->next->city, name) < 0)
	{
		head = head->next;
	}

	ptr->next = head->next;
	head->next = ptr;
}

int printa(countries head)
{
	while (head != NULL)
	{
		printf("%s\n", head->country);
		gradprinta(head->cityhead);
		head = head->next;
		printf("\n");
	}
}

int gradprinta(cities head)
{
	if (head == NULL)
	{
		return 0;
	}
	printf("%s %d\n", head->city, head->population);
	gradprinta(head->left);
	gradprinta(head->right);
}

int printb(countries head)
{
	if (head == NULL)
	{
		return 0;
	}
	printf("%s\n", head->country);
	gradprintb(head->cityhead->next);
	printb(head->left);
	printb(head->right);
}

int gradprintb(cities head)
{
	while (head != NULL)
	{
		printf("%s %d\n", head->city, head->population);
		head = head->next;
	}
	printf("\n");
}

int lobby(countries head)
{
	int pop;

	printf("Unesite populaciju:\n");
	scanf("%d", &pop);

	printf("\n");
	printf("Gradovi sa vecom populacijom:\n");

	while (head != NULL)
	{
		treesearch(head->cityhead, pop);
		head = head->next;
	}
}

int treesearch(cities head, int pop)
{
	if (head == NULL)
	{
		return 0;
	}

	if (head->population > pop)
	{
		printf("%s %d\n", head->city, head->population);
	}

	treesearch(head->left, pop);
	treesearch(head->right, pop);

}