#include <stdbool.h>

#define MAX_PLAYERS 4
#define MAX_PLAYER_NAME 8
#define MAX_CARD_HAND 11
#define	MAX_LEN	64

enum suits {clubs, diamonds, hearts, spades};
enum player_type {EA, HU};
//enum status {blackjack, bust, stand, hit} // double, surrender

typedef struct card {
	int suit;
	int id;
} card;

typedef struct player {
	int player_type;
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

#endif
