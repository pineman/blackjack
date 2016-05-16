#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <math.h>
#include "error.h"
#include "logic.h"
#include "ea.h"

/*
 * O formato do ficheiro de estratégia das EAs é:
 * 10 (HARD_LINES) linhas com 10 caracteres cada para as decisões Hard
 * uma linha em branco (um \n)
 * 7 (SOFT_LINES) linhas com 10 caracteres cada para as decisões Soft
 * os caracteres podem ser:
 *	H - hit
 *	S - stand
 *	R - surrender
 *	D - double senão stand
 *	E - double senão hit
 */

// TODO: Error checking no ficheiro de config das eas?
void write_matrix(Move ***matrix, FILE *file, int lines)
{
    char buffer[COLUMNS+2] = {0}; // COLUMNS + '\n' e '\0'
	*matrix = (Move **) ecalloc(lines, sizeof(Move *));

    for (int i = 0; i < lines; i++) {
		fgets(buffer, COLUMNS+2, file);
		// Se o caracter exatamente após COLUMNS colunas da linha não for \n,
		// sabemos que a linha não tem exatamente COLUMNS caracteres.
		if (buffer[COLUMNS] != '\n') {
			puts("Erro: Ficheiro de estratégia das EAs mal formatado.");
			exit(EXIT_FAILURE);
		}
        (*matrix)[i] = (Move *) ecalloc(10, sizeof(Move));
         for (int j = 0; j < COLUMNS; j++)
            (*matrix)[i][j] =  buffer[j];
    }
}

void destroy_matrix(Move **matrix, int lines)
{
    for (int i = 0; i < lines; i++)
		free(matrix[i]);

	free(matrix);
}

Strategy *read_strategy(char *filename)
{
	char check[2] = {0}; // \n e \0
    FILE *config_file = efopen(filename, "r");

    Strategy *strategy = (Strategy *) ecalloc(1, sizeof(Strategy));

    strategy->hard = NULL;
    strategy->soft = NULL;

    write_matrix(&strategy->hard, config_file, HARD_LINES);

    // Verificar \n de separação
    fgets(check, 2, config_file);
    if (check[0] != '\n') {
		puts("Erro: Ficheiro de estratégia das EAs mal formatado.");
		exit(EXIT_FAILURE);
	}

    write_matrix(&strategy->soft, config_file, SOFT_LINES);

	fclose(config_file);

    return strategy;
}

Move get_decision(Player *player, Card *house_card, Strategy *strategy)
{
    bool ace = false;
    int line = 0, column = 0;
    Stack *aux = player->cards;
    while(aux) {
        if (aux->card->id == 12)
            ace = true;
        aux = aux->next;
    }

    if (house_card->id > 0 && house_card->id < 8)
        column = house_card->id;
    else if (house_card->id >=8 && house_card->id < 12)
        column = 8;
    else if (house_card->id == 12)
        column = 9;

    if (ace) {
		// Escolheu-se que dois ases correspondem à primeira linha da matriz soft
		if (player->points == 12)
			line = 0;
        else if (player->points > 12 && player->points < 19)
            line = player->points - 13;
        else if (player->points >= 19)
            line = 6;

        return strategy->soft[line][column];
    }
    else {
        if (player->points >= 4 && player->points <= 8)
            line = 0;
        else if (player->points > 8 && player->points < 17)
            line = player->points - 8;
        else if (player->points >= 17)
            line = 9;

        return strategy->hard[line][column];
    }
}

// TODO: Temos de informar o utilizador da decisão da EA?
// fazemos print? messagebox? nada disto?
void ea_make_decision(List *players, Player *house, Megadeck *megadeck, Strategy *strategy)
{
	bool can_double = false;
	List *aux = find_active_player(players);
	Player *cur_player = (Player *) aux->payload;

	Card *house_card = house->cards->next->card;
	Move decision = get_decision(cur_player, house_card, strategy);

	printf("player->name = %s\n", cur_player->name);
	printf("house->card = %d\n", house_card->id);
	printf("EA->points = %d\n", cur_player->points);

	switch (decision) {
		case H:
			puts("Decisão: hit");
			player_hit(players, house, megadeck);
			break;

		case S:
			puts("Decisão: stand");
			stand(players, house, megadeck);
			break;

		case R:
			puts("Decisão: surrender");
			surrender(players, house, megadeck);
			break;

		case D:
			puts("Decisão: double");
			can_double = double_bet(players, house, megadeck);
			if (!can_double) {
				puts("Decisão: stand porque não double");
				stand(players, house, megadeck);
			}
			break;

		case E:
			puts("Decisão: double");
			can_double = double_bet(players, house, megadeck);
			if (!can_double) {
				puts("Decisão: hit porque não double");
				player_hit(players, house, megadeck);
			}
			break;

		default:
			// Isto nunca deverá acontecer
			puts("Erro: Decisão de EA inesperada.");
			exit(EXIT_FAILURE);
			break;
	}
	puts("");
}

void count_cards(Card *new_card, Megadeck *megadeck)
{
	if (new_card->id < 5)
		megadeck->round_count++;
	else if (new_card->id > 7)
		megadeck->round_count--;
}

// soma player->count com megadeck->round_count
void update_count(List *players, Megadeck *megadeck)
{
	puts("updating count");
	List *aux = players->next;
	Player *cur_player = NULL;

	while (aux) {
		cur_player = (Player *) aux->payload;
		if (cur_player->type == EA) {
			printf("b: EA->count = %d\n", cur_player->count);
			cur_player->count += megadeck->round_count;
			printf("a: EA->count = %d\n", cur_player->count);
		}
		aux = aux->next;
	}

	megadeck->round_count = 0;
}

void hi_lo(Player *player, Megadeck *megadeck)
{
	double decks_left = round(((double) megadeck->cards_left)/DECK_SIZE + 1);
	printf("decks_left = %f\n", decks_left);
	double true_count = round(player->count/decks_left);
	printf("true_count = %f\n", true_count);
	double modifier = pow(2, true_count);
	printf("modifier = %f\n", modifier);

	// TODO: o hi_lo tem por base a aposta original ou currente?
	if (player->type == EA)
		if (true_count != 0)
			// TODO: checkar se o jogador tem dinheiro para cobrir
			player->bet = player->orig_bet * modifier;

	printf("a: EA->bet = %d\n", player->bet);
}
