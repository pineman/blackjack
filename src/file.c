// Leitura e escrita de ficheiros
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "file.h"
#include "structs.h"

// TODO: fazer print da linha com erro

Config *read_config(char *filename)
{
	char *str;
	char buffer[MAX_LINE_LEN];

	Config *game_config = NULL;
	game_config = (Config *) malloc(sizeof(Config));

	FILE *fp = NULL;
	fp = fopen(filename, "r");

	if (fp == NULL) {
		puts("Erro: Impossivel abrir ficheiro");
		exit(EXIT_FAILURE);
	}

	fgets(buffer, MAX_LINE_LEN, fp);
	sscanf(buffer, "%d-%d", &(game_config->num_decks),
							&(game_config->num_players));

	if (game_config->num_decks > 8 || game_config->num_decks < 4){
		puts("Erro: numero de baralhos invalido!");
		exit(EXIT_FAILURE);
	}

	if (game_config->num_players > 4 || game_config->num_players < 1) {
		puts("Erro: numero de jogadores invalido!");;
		exit(EXIT_FAILURE);
	}


	for (int i=0; fgets(buffer, MAX_LINE_LEN, fp) != NULL && i < game_config->num_players; i++) {
		str = strtok(buffer, "-");
		if (!strcmp(str, "HU"))
			game_config->player_type[i] = HU;
		else if (!strcmp(str, "EA"))
			game_config->player_type[i] = EA;
		else {
			puts("Erro: Tipo de jogador invalido!");
			exit(EXIT_FAILURE);
		}

		str = strtok(NULL, "-");
		// TODO: player_name é no máximo MAX_PLAYER_NAME
		if (strlen(str) > MAX_PLAYER_NAME) {
			puts("Erro: Nome do jogador demasiado grande!");
			exit(EXIT_FAILURE);
		}
		strcpy(game_config->player_names[i], str);

		str = strtok(NULL, "\0");
		sscanf(str, "%d-%d", &game_config->money[i], &game_config->bets[i]);

		if (game_config->money[i] < 10 || game_config->money[i] > 500) {
			puts("Erro: Valor inicial de dinheiro invalido!");
			exit(EXIT_FAILURE);
		}

		if (game_config->bets[i] < 2 ||
			game_config->bets[i] > game_config->money[i] * 0.25)
		{
			puts("Valor da aposta invalido!");
			exit(EXIT_FAILURE);
		}
	}

	return game_config;
}

int main()
{
	Config *game_config = read_config("parametros.txt");
	printf("num_decks = %d\n", game_config->num_decks);
	printf("num_players = %d\n", game_config->num_players);
	for (int i = 0; i < game_config->num_players; i++) {
		printf("player_type[%d] = %s\n", i, game_config->player_type[i] == HU ? "HU" : "EA");
		printf("player_names[%d] = %s\n", i, game_config->player_names[i]);
		printf("money[%d] = %d\n", i, game_config->money[i]);
		printf("bets[%d] = %d\n", i, game_config->bets[i]);
	}
}
