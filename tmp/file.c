#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_PLAYERS 4
#define	MAX_LEN	64

//enum player type

typedef struct config {
	int num_decks;
	int num_players;
	char type_players[MAX_PLAYERS][4];
	char player_names[MAX_PLAYERS][MAX_LEN];
	int money[MAX_PLAYERS];
	int bets[MAX_PLAYERS];
} config;

config *read_config(char *filename);

int main()
{
	config *parameters = NULL;
	parameters = read_config("parametros.txt");

	printf("num_decks: %d\n", parameters->num_decks);
	for (int i = 0; i < MAX_PLAYERS; i++)
		printf("money[%d]: %d\n", i, parameters->money[i]);

	return EXIT_SUCCESS;
}

config *read_config(char *filename)
{
	char *str;
	char *token;
	static char *buffer;
	buffer = (char *) malloc(MAX_LEN*sizeof(char));

	config *parameters = NULL;
	parameters = (config *) malloc(sizeof(config));

	FILE *fp = NULL;
	fp = fopen(filename, "r");

	if (fp == NULL) {
		puts("Erro: Impossivel abrir ficheiro");
		exit(EXIT_FAILURE);
	}

	fgets(buffer, MAX_LEN, fp);
	sscanf(buffer, "%d-%d", &(parameters->num_decks),
							&(parameters->num_players));

	if (parametros->num_deck > 8 || parametros->num_deck < 4){
		puts("Erro: numero de baralhos invalido!");
		exit(EXIT_FAILURE);

	if (parametos)
	for (int i=0; fgets(buffer, MAX, fp) != NULL; i++) {
		strcpy(parameters->type_players[i], strsep(&buffer, "-"));
		strcpy(parameters->player_names[i], strsep(&buffer, "-"));
		sscanf(buffer, "%d-%d", &parameters->money[i], &parameters->bets[i]);

	}

	return parameters;
}
