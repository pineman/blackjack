#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
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
    char buffer[COLUMNS+2] = {0};
	*matrix = (Move **) ecalloc(lines, sizeof(Move *));

    for (int i = 0; i < lines; i++) {
		fgets(buffer, COLUMNS+2, file); // TODO: se fgets der overflow é logo erro
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
    FILE *config_file = efopen(filename, "r");

    Strategy *strategy = (Strategy *) ecalloc(1, sizeof(Strategy));

    strategy->hard = NULL;
    strategy->soft = NULL;

    write_matrix(&strategy->hard, config_file, HARD_LINES);

    // Consumir \n de separação ('\n' => 1 char => 1 byte!)
    fseek(config_file, 1, SEEK_CUR);

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

void count_strategy(Player *player, Card *new_card, Megadeck *megadeck)
{
	if (new_card->id < 5)
		megadeck->count++;
	else if (new_card->id > 7)
		megadeck->count--;

	player->count = megadeck->count;
}

// TODO: lá vamos nós falar com o ascenso outra vez
void hi_lo(List *players, Megadeck *megadeck)
{
	int decks_left = megadeck->cards_left/DECK_SIZE;
	int modifier = megadeck->count/decks_left;

	List * aux = players->next;
	while (aux) {
		Player *cur_player = (Player *)  aux->payload;
		if (cur_player->type == EA && modifier > 0)
			cur_player->bet = cur_player->orig_bet * (modifier+1);
		aux = aux->next;
	}
}
