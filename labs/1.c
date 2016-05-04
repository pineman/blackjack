#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define	MAX	64
#define	ACE 12

typedef struct {
	int id;
	int suit;
} card;

int alloc_cards(char *file, card **deck)
{
	char buffer[MAX];
	int i=0;

	FILE *fp = NULL;
	fp = fopen(file, "r");
	if (fp == NULL) {
		puts("Erro: impossivel ler ficheiro!");
		exit(EXIT_FAILURE);
	}

	for(i=0; fgets(buffer, MAX, fp) != NULL; i++) {
		*deck = realloc(*deck, (i+1)*sizeof(card));
		sscanf(buffer, "%d-%d", &((*deck)[i].id), &((*deck)[i].suit));
		printf("%d\n", (*deck)[i].id);
	}

	return i;
}

int seek_ace(card *array, int count)
{
	int ace_count=0;
	for (int i=0; i<count; i++) {
		if (array[i].id == ACE)
			ace_count++;
	}

	

	return ace_count;
}
int main()
{
	int ace_count = 0;
	int count = 0;
	card *deck=NULL;
	char file[MAX];
	printf("Carregar as cartas do ficheiro: ");
	scanf("%s", file);

	count = alloc_cards(file, &deck);

	printf("Numero de ases: %d\n", seek_ace(deck, count));

	return EXIT_SUCCESS;
}