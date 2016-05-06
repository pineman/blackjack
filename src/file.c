// Leitura e escrita de ficheiros
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "file.h"
#include "logic.h"

// TODO: fazer print da linha com erro
Config *read_config(char *filename)
{
	char buffer[MAX_LINE_LEN];

	Config *config = NULL;
	config = (Config *) malloc(sizeof(Config));

	FILE *fp = NULL;
	fp = fopen(filename, "r");

	if (fp == NULL) {
		puts("Erro: Impossivel abrir ficheiro");
		exit(EXIT_FAILURE);
	}

	fgets(buffer, MAX_LINE_LEN, fp);
	sscanf(buffer, "%d-%d", &(config->num_decks), &(config->num_players));

	if (config->num_decks > 8 || config->num_decks < 4){
		puts("Erro: numero de baralhos invalido!");
		exit(EXIT_FAILURE);
	}

	if (config->num_players > 4 || config->num_players < 1) {
		puts("Erro: numero de jogadores invalido!");;
		exit(EXIT_FAILURE);
	}

	for (int i=0; fgets(buffer, MAX_LINE_LEN, fp) != NULL && i < config->num_players; i++)
		config = read_player(buffer, config, i);

	return config;
}

Config *read_player(char *line, Config *config, int count)
{
	char *str = strtok(line, "-");
	if (!strcmp(str, "HU"))
		config->player_type[count] = HU;
	else if (!strcmp(str, "EA"))
		config->player_type[count] = EA;
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
	strcpy(config->player_names[count], str);
	str = strtok(NULL, "\0");
	sscanf(str, "%d-%d", &config->money[count], &config->bets[count]);
	if (config->money[count] < 10 || config->money[count] > 500) {
		puts("Erro: Valor inicial de dinheiro invalido!");
		exit(EXIT_FAILURE);
	}

	if (config->bets[count] < 2 ||
		config->bets[count] > config->money[count] / 4)
	{
		puts("Valor da aposta invalido!");
		exit(EXIT_FAILURE);
	}

	return config;
}
