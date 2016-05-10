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
void stats(List *players, Player *house)
{
    FILE *stats = NULL;
    stats = fopen("stats.txt" , "w");
    if (!stats) 
        exit(EXIT_FAILURE);
    
    List *aux = players->next;
    Player *cur_player = NULL;
    fprintf(stats, "Jogador\t\tTipo\tVitorias\tEmpates\tDerrotas\tDinheiro\n");
    while (aux) {
        cur_player = (Player *) aux->payload;
        if (cur_player->type == VA) {
            aux = aux->next;
            continue;
        }
        fprintf(stats, "%s\t", cur_player->name);
        if (strlen(cur_player->name) < 8)
            fprintf(stats, "\t");
        if (cur_player->type == EA)
            fprintf(stats, "EA\t");
        else if (cur_player->type == HU)
            fprintf(stats, "HU\t");
        fprintf(stats, "%d\t\t", cur_player->wins);
        fprintf(stats, "%d\t", cur_player->ties);
        fprintf(stats, "%d\t\t", cur_player->losses);
        fprintf(stats, "%d\n", cur_player->money);
        aux = aux->next;
    }

    fprintf(stats, "A casa lucrou: %d\n", house->money);
} 
