#ifndef LOGIC_H
#define LOGIC_H

#include <stdbool.h>
#include "list.h"

#define MAX_PLAYERS 4
#define MAX_LINE_LENGTH 64
#define	DECK_SIZE 52
#define SUIT_SIZE 13
#define MAX_CARD_HAND 11
#define MAX_PLAYER_NAME 9

enum suits {clubs, diamonds, hearts, spades};
enum player_type {EA, HU};
//enum status {blackjack, bust, stand, hit} // double, surrender

typedef struct Card {
	int suit;
	int id;
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

void stack_push(Stack **sp, Card *card);
Card *stack_pop(Stack **sp);
int give_card(List *megadeck, int *cards_left, int num_decks, Player *player);
int create_megadeck(List *megadeck, int num_decks);
void destroy_list(List *head);
void destroy_stack(Stack **cards);
void print_list(List *megadeck);
void print_reverse_stack(Stack *sp);

#endif
