#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define	MAX	64

typedef struct config {
	int num_decks;
	int num_players;
	char type_players[4][4];
	char player_names[4][MAX];
	int money[4];
	int bets[4];
} config;

config *read_config(char *filename);
	

int main()
{
	config *parameter = NULL;
	parameter = read_config("parametros.txt");

	return EXIT_SUCCESS;
}

config *read_config(char *filename)
{
	char *str;
	char *token;
	static char *buffer;
	buffer = (char *) malloc(64*sizeof(char));


	config *parameters = NULL;
	parameters = (config *) malloc(sizeof(config));

	//read file
	FILE *fp = NULL;
	fp = fopen(filename, "r");
	
	if (fp == NULL) {
		puts("Erro: Impossivel abrir ficheiro");
		exit(EXIT_FAILURE);
	}

	fgets(buffer, MAX, fp);
	sscanf(buffer, "%d-%d", &(parameters->num_decks), 
							&(parameters->num_players));

	for (int i=0; fgets(buffer, MAX, fp) != NULL; i++) {

		strcpy(parameters->type_players[i], strsep(&buffer, "-"));
		strcpy(parameters->player_names[i], strsep(&buffer, "-"));

		sscanf(buffer, "%d-%d", &parameters->money[i], &parameters->bets[i]);
	}
	
	return parameters;
}
