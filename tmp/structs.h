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
	//stack *cards;
	int wins;
	int losses;
	int draws;
	//enum status;
} player;

typedef struct config {
	int num_decks;
	int num_players;
	// Para cada jogador, arrays de no máximo 4
	bool human[4];
	char player_names[4][MAX_PLAYER_NAME+1];
	int initial_money[4];
	int initial_bet[4];
}
