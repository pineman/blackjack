#include <stdbool.h>

#define MAX_PLAYERS 4
#define MAX_LINE_LENGTH 64
#define MAX_PLAYER_NAME 9

enum suits {clubs, diamonds, hearts, spades};
enum player_type {EA, HU};
//enum status {blackjack, bust, stand, hit} // double, surrender

typedef struct Card {
	int suit; //0-3
	int id;	//0-12
} Card;

typedef struct Stack {
	Card *card;
	struct Stack *next;
} Stack;

typedef struct Player {
	int player_type;
	char name[MAX_PLAYER_NAME+1];
	int position;
	bool playing;
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

typedef struct House {
	int money;
	Stack *cards;
	int num_cards;
	int points;
} House;

#endif
