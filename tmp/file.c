// Leitura e escrita de ficheiros
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_PLAYERS 4
#define	MAX_LEN	64

enum type_player {EA, HU};

typedef struct config {
	int num_decks;
	int num_players;
	int player_type[MAX_PLAYERS];
	char player_names[MAX_PLAYERS][MAX_LEN];
	float money[MAX_PLAYERS];
	float bets[MAX_PLAYERS];
} config;

config *read_config(char *filename)
{
	char *str;
	char *buffer;
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

	if (parameters->num_decks > 8 || parameters->num_decks < 4){
		puts("Erro: numero de baralhos invalido!");
		exit(EXIT_FAILURE);
	}

	if (parameters->num_players > 4 || parameters->num_players < 1) {
		puts("Erro: numero de jogadores invalido!");;
		exit(EXIT_FAILURE);
	}

	for (int i=0; fgets(buffer, MAX_LEN, fp) != NULL; i++) {
		str = strtok(buffer, "-");
		if (!strcmp(str, "HU"))
			parameters->player_type[i] = HU;
		else if (!strcmp(str, "EA"))
			parameters->player_type[i] = EA;
		else {
			puts("Erro: Tipo de jogador invalido!");
			exit(EXIT_FAILURE);
		}
		
		str = strtok(NULL, "-");
		strcpy(parameters->player_names[i], str);

		str = strtok(NULL, "\0");
		sscanf(str, "%f-%f", &parameters->money[i], &parameters->bets[i]);
		
		if (parameters->money[i] < 10 || parameters->money[i] > 500) {
			puts("Erro: Valor inicial de dinheiro invalido!");
			exit(EXIT_FAILURE);
		}

		if (parameters->bets[i] < 2 || 
			parameters->bets[i] > parameters->money[i] * 0.25) 
		{
			puts("Valor da aposta invalido!");
			exit(EXIT_FAILURE);
		}
	}

	return parameters;
}

int main()
{
	return EXIT_SUCCESS;
}