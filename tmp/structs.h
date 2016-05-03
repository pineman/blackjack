#include <stdbool.h>
#define MAX_PLAYER_NAME 8

enum suits {clubs, diamonds, hearts, spades};
//enum status {blackjack, bust, stand, hit} // double, surrender

typedef struct card {
	enum suits suit;
	int id;
} card;

typedef struct player {
	bool human;
	char name[MAX_PLAYER_NAME+1];
	int position;
	bool playing;
	int money;
	int bet;
	int points;
	card cards[MAX_CARD_HAND];
	int num_cards;
	int wins;
	int losses;
	int draws;
	//enum status;
} player;

typedef struct config {
	int num_decks;
	int num_players;
	int player_type[MAX_PLAYERS];
	char player_names[MAX_PLAYERS][MAX_LEN];
	float money[MAX_PLAYERS];
	float bets[MAX_PLAYERS];
} config;
