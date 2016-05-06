// Leitura e escrita de ficheiros
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "file.h"
#include "logic.h"

// TODO: fazer print da linha com erro

Config *read_line(char *line, Config *game_config, int count)
{
	char *str = strtok(line, "-");
	if (!strcmp(str, "HU"))
		game_config->player_type[count] = HU;
	else if (!strcmp(str, "EA"))
		game_config->player_type[count] = EA;
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
	strcpy(game_config->player_names[count], str);
	str = strtok(NULL, "\0");
	sscanf(str, "%d-%d", &game_config->money[count], &game_config->bets[count]);
	if (game_config->money[count] < 10 || game_config->money[count] > 500) {
		puts("Erro: Valor inicial de dinheiro invalido!");
		exit(EXIT_FAILURE);
	}

	if (game_config->bets[count] < 2 ||
		game_config->bets[count] > game_config->money[count] / 4)
	{
		puts("Valor da aposta invalido!");
		exit(EXIT_FAILURE);
	}

	return game_config;

}

Config *read_config(char *filename)
{
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


	for (int i=0; fgets(buffer, MAX_LINE_LEN, fp) != NULL && i < game_config->num_players; i++)
		game_config = read_line(buffer, game_config, i);

	return game_config;
}

