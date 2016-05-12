#ifndef LOGIC_H
#define LOGIC_H

#include <stdbool.h>
#include "list.h"

#define MAX_PLAYERS 4
#define MAX_LINE_LENGTH 64
#define	DECK_SIZE 52
#define SUIT_SIZE 13
#define MAX_PLAYER_NAME 8

// Entidade Artificial, Humano, Vazio
typedef enum {EA, HU, VA} Type;
// waiting, blackjack, busted, stand'ed', surrender
typedef enum {WW, BJ, BU, ST, SU} Status;

typedef struct Card {
	int suit;
	int id;
} Card;

typedef struct Stack {
	Card *card;
	struct Stack *next;
	struct Stack *prev;
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

int init_game(Config *config, List *players);

void stack_push(Stack **sp, Card *card);
Card *stack_pop(Stack **sp);


int give_card(Player *player, Megadeck *megadeck);
int create_megadeck(Megadeck *megadeck);

void new_game(List *players, Player *house, Megadeck *megadeck);
void clear_cards(List *players, Player *house);
void distribute_cards(List *players, Player *house, Megadeck *megadeck);
void find_playing(List *players, Player *house);

List *find_ingame_player(List *players);
List *find_active_player(List *players);

void quit_game(List *players, bool *quit);
void surrender(List *players, Player *house, Megadeck *megadeck);
void double_bet(List *players, Player *house, Megadeck *megadeck);
void bet(List *players);
void add_player(List *players, List *old_players);
void stand(List *players, Player *house, Megadeck *megadeck);
void player_hit(List *players, Player *house, Megadeck *megadeck);
void house_hit(Player *house, Megadeck *megadeck);

void pay_bets(List *players, Player *house);

void count_points(Player *player);
int point_index(int id);

void destroy_list(List *head);
void destroy_stack(Stack **cards);
void destroy_players_list(List *players);

#endif
