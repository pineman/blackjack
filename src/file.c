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

int get_new_bet(List *players)
{
	char buffer[MAX_PLAYER_NAME+2];
	List *aux = players->next;
	Player *cur_player = NULL;

	printf("Insira o nome do jogador a modificar a aposta: ");
	fgets(buffer, sizeof(buffer), stdin);
	int newline = (int) strcspn(buffer, "\n");

	if (newline == 9) {
		puts("O nome do jogador tem no máximo 8 caracteres.");
		return -1;
	}
	else
		buffer[newline] = '\0';

	bool found_player = false;
	while (aux && !found_player) {
		cur_player = (Player *) aux->payload;
		if (strcmp(buffer, cur_player->name) == 0 && !found_player)
			found_player = true;
		else
			aux = aux->next;
	}

	if (!aux) {
		puts("Jogador não encontrado.");
		return -1;
	}

	cur_player = (Player *) aux->payload;
	printf("Insira o novo valor da aposta do jogador %s: ", cur_player->name);
	fgets(buffer, sizeof(buffer), stdin);
	long new_bet = strtol(buffer, NULL, 10);
	// 25% ou 100% do dinheiro?
	// o dinheiro do jogador está (essencialmente) garantido
	// de estar abaixo de INT_MAX (a não ser que se jogue mesmo muito)
	// fazendo com bet de [1, money]
	if (new_bet > cur_player->money || new_bet < 1) {
		puts("Nova aposta inválida.");
		return -1;
	}

	return new_bet;
}

// TODO: parse new player correctly!
Player *get_new_player(int pos)
{
	Type type = HU;
	char name[MAX_PLAYER_NAME+1] = "novo";
	int money = 0;
	int bet = 0;
	Player *new_player = NULL;

	printf("Escolheu o jogador na posição %d.\n", pos);

	new_player = (Player *) calloc((size_t) 1, sizeof(Player));

	new_player->type = type;
	strcpy(new_player->name, name);
	new_player->ingame = true; // certificamo-nos que money >= bet
	new_player->money = money;
	new_player->bet = bet;

	return new_player;
}

void write_stats(List *players, Player *house, List *old_players, bool *quit)
{
    List *aux = find_active_player(players);
    if (aux)
        return;

    FILE *stats = NULL;
    stats = fopen("stats.txt" , "w");
    if (!stats)
        exit(EXIT_FAILURE);

    fprintf(stats, "Jogador\t\tTipo\tJogos\tVitorias\tEmpates\tDerrotas\tDinheiro\n");

	write_stats_players(stats, players);
	write_stats_players(stats, old_players);

	if (house->money < 0)
		fprintf(stats, "A casa perdeu: %d €\n", -1*house->money);
	else if (house->money > 0)
		fprintf(stats, "A casa ganhou: %d €\n", house->money);
	else if (house->money == 0)
		fprintf(stats, "A casa não ganhou nem perdeu dinheiro.");

	*quit = true;
}

void write_stats_players(FILE *stats, List *players)
{
	List *aux = players->next;
	Player *cur_player = NULL;
	// TODO: se escrevermos cada estatística para cada jogador
	// na sua própria linha evitamos problemas com tabs
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
        fprintf(stats, "%d\t", cur_player->wins+cur_player->losses+cur_player->ties);
        fprintf(stats, "%d\t\t", cur_player->wins);
        fprintf(stats, "%d\t", cur_player->ties);
        fprintf(stats, "%d\t\t", cur_player->losses);
        fprintf(stats, "%d €\n", cur_player->money);
        aux = aux->next;
    }
}
