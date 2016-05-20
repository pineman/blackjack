#define MAX_PLAYER_NAME 8
#define MAX_PLAYERS 4
#include "list.h"

// Tipo de jogador: Entidade Artificial, Humano, Vazio
typedef enum {EA, HU, VA} Type;

// Estado do jogador: waiting, blackjack, busted, stand'ed', surrender
typedef enum {WW, BJ, BU, ST, SU} Status;

// Addplayer: OK (sem erro), OUT: o utilizador clicou fora da área dos jogadores
// NOTEMPTY: o utilizador clicou num jogador que ainda está a jogar
typedef enum {OK, OUT, NOTEMPTY} AddPlayerError;

// Codificação das ações das EAs:
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
	// guarda a contagem das cartas para o hi-lo
	// (apenas usada se o jogador for EA)
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
	// guarda a contagem das cartas duma ronda apenas para o hi-lo das EAs
	int round_count;
} Megadeck;

typedef struct Strategy {
    Move **hard;
    Move **soft;
} Strategy;
