#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "list.h"

// estes não ficam aqui!!
#define	DECK_SIZE 52
#define SUIT_SIZE 13
#define MAX_CARD_HAND 11

enum suits {clubs, diamonds, hearts, spades};

typedef struct card {
	enum suits suit;
	int id; // 0 a 12
} card;

int create_megadeck(list *head, int num_decks);
void destroy_megadeck(list *head);
int give_card(
	list *megadeck, int *cards_left, int num_decks,
	card *cards[MAX_CARD_HAND], int *num_cards);

int main()
{
	srand(time(NULL));
	list dummy_head = {NULL, NULL, NULL};
	list *megadeck = &dummy_head;
	int cards_left = 0;
	int num_decks = 2;
	card *cards[MAX_CARD_HAND] = {0};
	int num_cards = 0;

	give_card(megadeck, &cards_left, num_decks, cards, &num_cards);
	give_card(megadeck, &cards_left, num_decks, cards, &num_cards);

	for (int i = 0; i < num_cards; i++)
		printf("suit: %d id: %d\n", cards[i]->suit, cards[i]->id);

	destroy_megadeck(megadeck);
}

int create_megadeck(list *megadeck, int num_decks)
{
	int total_cards = 0;

	for(int i = 0; i < num_decks; i++)
		for(int j = clubs; j <= spades; j++)
			for(int k = 0; k < SUIT_SIZE; k++) {
				//cur_card = cards[i*DECK_SIZE + j*SUIT_SIZE + k];
				card *cur_card = (card *) calloc(1, sizeof(card));
				cur_card->suit = j;
				cur_card->id = k;
				list_append(megadeck, cur_card);
			}

	total_cards = num_decks * DECK_SIZE;
	return total_cards;
}

void destroy_megadeck(list *head)
{
	list *aux = head->next; // dummy head
	list *tmp = NULL;
	while (aux) {
		tmp = aux;
		aux = tmp->next;
		free(tmp->payload);
		free(tmp);
	}
}

int give_card(
	list *megadeck, int *cards_left, int num_decks,
	card *cards[MAX_CARD_HAND], int *num_cards)
{
	int random = 0;

	if (*num_cards == MAX_CARD_HAND)
		// TODO: o jogador não pode receber mais cartas
		return -1;

	if (!*cards_left)
		*cards_left = create_megadeck(megadeck, num_decks);

	random = rand() % *cards_left;

	list *aux = megadeck->next; // dummy head
	for (int i = 0; i < random; i++) {
		if (aux->next)
			aux = aux->next;
		else
			// TODO: a carta que queremos dar não existe
			return -1;
	}

	cards[*num_cards] = (card *) aux->payload;
	++*num_cards;
	list_remove(aux);
	--*cards_left;
	return 0;
}
