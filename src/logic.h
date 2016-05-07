#ifndef LOGIC_H
#define LOGIC_H

#include <stdbool.h>
#include "list.h"

#define MAX_PLAYERS 4
#define MAX_LINE_LENGTH 64
#define	DECK_SIZE 52
#define SUIT_SIZE 13
#define MAX_CARD_HAND 11
#define MAX_PLAYER_NAME 8

typedef enum {EA, HU} Type;
typedef enum {NA, BJ, BU, ST} Status; // double, surrender

/* 0-active
 * 2-blackjack
 * 3-bust
 * 4-bankrupt
 * 5-stand
 * 6-hit
 */
/*typedef enum modes {active, blackjack, bust, bankrupt, stand, hit} Status;*/

typedef struct Card {
	int suit;
	int id;
} Card;

typedef struct Stack {
	Card *card;
	struct Stack *next;
} Stack;



typedef struct Player {
	Type type;
	bool playing;
	char name[MAX_PLAYER_NAME+1];
	int position;
    Status status;
	int money;
	int bet;
	Stack *cards;
	int num_cards;
	int points;
	int wins;
	int losses;
	int draws;
	//enum status;
} Player;

typedef struct Config {
	int num_decks;
	int num_players;
	int player_type[MAX_PLAYERS];
	char player_names[MAX_PLAYERS][MAX_PLAYER_NAME+1];
	int money[MAX_PLAYERS];
	int bets[MAX_PLAYERS];
} Config;

int init_game(Config *config, List *players);
void stack_push(Stack **sp, Card *card);
Card *stack_pop(Stack **sp);
int give_card(Player *player, List *megadeck, int *cards_left, const int num_decks);
int create_megadeck(List *megadeck, const int num_decks);
void new_game(List *players, Player *house, List *megadeck, int *cards_left, const int num_decks);
void destroy_list(List *head);
void destroy_stack(Stack **cards);
void count_points(Player *player);
int point_index(int id);

#endif
