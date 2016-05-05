#include <stdbool.h>
#include "stack.h"

#define MAX_PLAYERS 4
#define MAX_PLAYER_NAME 8

enum suits {clubs, diamonds, hearts, spades};
enum player_type {EA, HU};
//enum status {blackjack, bust, stand, hit} // double, surrender

typedef struct card {
	int suit;
	int id;
} card;

struct player_struct {
	int player_type;
	char name[MAX_PLAYER_NAME+1];
	int position;
	bool playing;
	int money;
	int bet;
	int points;
	stack *cards;
	int num_cards;
	int wins;
	int losses;
	int draws;
	//enum status;
};
static struct player_struct default_player = {0, {0}, 0, false, 0, 0, 0, NULL, 0, 0, 0, 0};
typedef struct player_struct player;

typedef struct config {
	int num_decks;
	int num_players;
	int player_type[MAX_PLAYERS];
	char player_names[MAX_PLAYERS][MAX_PLAYER_NAME+1];
	int money[MAX_PLAYERS];
	int bets[MAX_PLAYERS];
} config;

#endif
