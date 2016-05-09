#ifndef LOGIC_H
#define LOGIC_H

#include <stdbool.h>
#include "list.h"

#define MAX_PLAYERS 4
#define MAX_LINE_LENGTH 64
#define	DECK_SIZE 52
#define SUIT_SIZE 13
#define MAX_PLAYER_NAME 8

typedef enum {EA, HU} Type;
// waiting, blackjack, busted, stand'ed'
typedef enum {WW, BJ, BU, ST, SU} Status; // double, surrender

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
	char name[MAX_PLAYER_NAME+1];
	bool ingame;
	bool playing;
    Status status;
	int money;
	int bet;
	Stack *cards;
	int num_cards;
	int points;
	int wins;
	int losses;
	int ties;
} Player;

typedef struct Config {
	int num_decks;
	int num_players;
	int player_type[MAX_PLAYERS];
	char player_names[MAX_PLAYERS][MAX_PLAYER_NAME+1];
	int money[MAX_PLAYERS];
	int bets[MAX_PLAYERS];
} Config;

typedef struct Megadeck {
    int cards_left;
    const int num_decks;
    List *deck;
} Megadeck;

int   init_game(Config *config, List *players);

void stack_push(Stack **sp, Card *card);
Card *stack_pop(Stack **sp);


int give_card(Player *player, Megadeck *megadeck);
int create_megadeck(Megadeck *megadeck);

void new_game(List *players, Player *house, Megadeck *megadeck);
void new_game_house(Player *house, Megadeck *megadeck);
void new_game_players(List *players, Player *house, Megadeck *megadeck);

List *find_active_player(List *players);
void stand(List *players, Player *house, Megadeck *megadeck);
void player_hit(List *players, Player *house, Megadeck *megadeck);
void house_hit(Player *house, Megadeck *megadeck);

void pay_bets(List *players, Player *house);

void count_points(Player *player);
int point_index(int id);

void destroy_list(List *head);
void destroy_stack(Stack **cards);

#endif
