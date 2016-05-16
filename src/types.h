#define MAX_PLAYER_NAME 8
#define MAX_PLAYERS 4
#include "list.h"

// Entidade Artificial, Humano, Vazio
typedef enum {EA, HU, VA} Type;

// waiting, blackjack, busted, stand'ed', surrender
typedef enum {WW, BJ, BU, ST, SU} Status;

// Okay, user clicked outside playerarea, user clicked on valid player
typedef enum {OK, OUT, NOTEMPTY} AddPlayerError;

// hit, stand, surrender, double->hit, double->stand
typedef enum {H='H', S='S', R='R', D='D', E='E'} Move;

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
	int orig_bet;
	Stack *cards;
	int num_cards;
	int points;
	int wins;
	int losses;
	int ties;
	// guarda a contagem das cartas para o hi-lo no caso do jogador ser EA
	int count;
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
	// guarda a contagem das cartas duma ronda para o hi-lo das EAs
	int round_count;
} Megadeck;

typedef struct Strategy {
    Move **hard;
    Move **soft;
} Strategy;
