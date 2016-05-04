#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define	MAX	64
#define	ACE 12

typedef struct {
	int id;
	int suit;
} card;

int alloc_cards(char *file, card **deck, int num_cards)
{
	char buffer[MAX];
	int i=0;

	FILE *fp = NULL;
	fp = fopen(file, "r");
	if (fp == NULL) {
		puts("Erro: impossivel ler ficheiro!");
		exit(EXIT_FAILURE);
	}

	for(i=num_cards; fgets(buffer, MAX, fp) != NULL; i++) {
		*deck = realloc(*deck, (i+1)*sizeof(card));
		sscanf(buffer, "%d-%d", &((*deck)[i].id), &((*deck)[i].suit));
	}

	return i;
}

/*int seek_ace(card *array, int count)
{
	int ace_count=0;
	for (int i=0; i<count; i++) {
		if (array[i].id == ACE)
			ace_count++;
	}

	return ace_count;
}*/

void print_deck(card *deck, int count)
{
	for (int i=0; i<count; i++) {
		printf("suit: %d\tid:%d\n", (deck+i)->suit, (deck+i)->id);
	}
}

int main()
{
	int ace_count = 0;
	int count = 0;
	int num_files;
	card *deck=NULL;

	printf("Numero de ficheiros a ler: ");
	scanf("%d", &num_files);
	char **file = (char **) malloc(num_files * sizeof(char*));

	for (int i=0; i<num_files; i++) {
		file[i] = (char*) malloc(MAX * sizeof(char));
		printf("Carregar cartas do ficheiro: ");
		scanf("%s", file[i]);
		count = alloc_cards(file[i], &deck, count);
	}

	print_deck(deck, count);


	return EXIT_SUCCESS;
}