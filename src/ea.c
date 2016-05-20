/* O sistema de ajuste de apostas utilizado neste projecto segue as instruções
 * explicadas aqui: http://casinogambling.about.com/od/blackjack/a/hilo.htm
 */

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
 * 8 (SOFT_LINES) linhas com 10 caracteres cada para as decisões Soft
 * os caracteres podem ser:
 *	H - hit
 *	S - stand
 *	R - surrender
 *	D - double senão hit
 *	E - double senão stand
 */

void write_matrix(Move ***matrix, FILE *file, int lines)
{
    char buffer[COLUMNS+2] = {0}; // COLUMNS + '\n' e '\0'
	*matrix = (Move **) ecalloc(lines, sizeof(Move *));

    for (int i = 0; i < lines; i++) {
		fgets(buffer, COLUMNS+2, file);
		// Se o caracter exatamente após COLUMNS colunas da linha não for \n,
		// sabemos que a linha não tem exatamente COLUMNS caracteres.
		if (buffer[COLUMNS] != '\n') {
			fprintf(stderr, "Erro: Ficheiro de estratégia das EAs mal formatado.\n");
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


// Ler a estrategia do ficheiro de configuração
Strategy *read_strategy(char *filename)
{
	char check[2] = {0}; // \n e \0
    FILE *config_file = efopen(filename, "r");

    Strategy *strategy = (Strategy *) ecalloc(1, sizeof(Strategy));

    strategy->hard = NULL;
    strategy->soft = NULL;

    write_matrix(&strategy->hard, config_file, HARD_LINES);

    // Verificar \n de separação das matrizes
    fgets(check, 2, config_file);
    if (check[0] != '\n') {
		fprintf(stderr, "Erro: Ficheiro de estratégia das EAs mal formatado.\n");
		exit(EXIT_FAILURE);
	}

    write_matrix(&strategy->soft, config_file, SOFT_LINES);

	fclose(config_file);

    return strategy;
}

/*
 * Decide que matriz utilizar: hard ou soft
 * Calcula a coluna e linha da matriz
 * Retorna a decisão a tomar
 */
Move get_decision(Player *player, Card *house_card, Strategy *strategy)
{
    bool ace = false;
    int line = 0, column = 0;
    Stack *aux = player->cards;

    //Verificar se a ases
    while(aux) {
        if (aux->card->id == 12)
            ace = true;
        aux = aux->next;
    }

    // Calcular a coluna da matriz
    if (house_card->id > 0 && house_card->id < 8)
        column = house_card->id;
    else if (house_card->id >=8 && house_card->id < 12)
        column = 8;
    else if (house_card->id == 12)
        column = 9;

    // Calcular a linha da matriz soft
    if (ace) {
		// Dois ases correspondem à primeira linha da matriz soft
		if (player->points == 12)
			line = 0;
        else if (player->points > 12 && player->points < 19)
            line = player->points - 12;
        else if (player->points >= 19)
            line = 7;

        return strategy->soft[line][column];
    }
    // Calcular a coluna da matriz hard
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


/*
 * Encontra proximo jogador
 * Usa o valor de retorno de get_decision para escolher a proxima ação
 */
void ea_make_decision(List *players, Player *house, Megadeck *megadeck, Strategy *strategy)
{
	bool can_double = false;
	List *aux = find_active_player(players);
	Player *cur_player = (Player *) aux->payload;

	Card *house_card = house->cards->next->card;
	Move decision = get_decision(cur_player, house_card, strategy);

	switch (decision) {
		case H:
			player_hit(players, house, megadeck);
			break;

		case S:
			stand(players, house, megadeck);
			break;

		case R:
			surrender(players, house, megadeck);
			break;


		case D:
			can_double = double_bet(players, house, megadeck);
			if (!can_double) {
				player_hit(players, house, megadeck);
			}
			break;
    	case E:
			can_double = double_bet(players, house, megadeck);
			if (!can_double) {
				stand(players, house, megadeck);
			}
			break;

		default:
			// Isto nunca deverá acontecer
			fprintf(stderr, "Erro: Decisão de EA inesperada.\n");
			exit(EXIT_FAILURE);
			break;
	}
}


/*
 * Cada jogador começa com a contagem = 0
 * Em cada ronda são contadas as cartas e no fim da ronda ao count dos jogadores
 * é somada a contagem da ronda
 */

//Conta cartas segundo a estrategia hi-lo
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
	List *aux = players->next;
	Player *cur_player = NULL;

	while (aux) {
		cur_player = (Player *) aux->payload;
		if (cur_player->type == EA)
			cur_player->count += megadeck->round_count;

		aux = aux->next;
	}

	megadeck->round_count = 0;
}

/* Altera a bet do jogador
 * A aposta original do jogador é uma unidade
 * true_count = contagem / numero de baralhos
 * A nova aposta no jogador é igual a 2*true_count unidades
 * Se true_count <= 0 a nova aposta e igual a 1 unidades
 */
void hi_lo(Player *player, Megadeck *megadeck)
{
    double new_bet = 0;
	double decks_left = round(((double) megadeck->cards_left)/DECK_SIZE + 1);
	int true_count = round(player->count/decks_left);

    if (true_count <= 0)
		new_bet = player->orig_bet;
    else
        new_bet = 2 * true_count * player->orig_bet;


    if (player->type == EA) {
		if (player->money > new_bet)
			player->bet = new_bet;
		else
			player->bet = player->money;
	}
}
