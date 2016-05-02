#include <stdio.h>
#include <stdlib.h>
#include "list.h"
#include "stack.h"

// estes não ficam aqui!!
#define	DECK_SIZE	52
#define SUIT_SIZE	13

enum suits {clubs, diamonds, hearts, spades};

typedef struct card {
	enum suits suit;
	int id; // 0 a 12
} card;

int create_megadeck(list *head, int num_decks);
void destroy_megadeck(list *head);
int give_card(list *megadeck, stack **player_cards, int cards_left);

int main()
{
	srand(123);
	list dummy_head = {NULL, NULL, NULL};
	list *megadeck = &dummy_head;

	int cards_left = 0;
	cards_left = create_megadeck(megadeck, 2);

	//list *aux = megadeck->next; // dummy head
	//while (aux) {
	//	printf("suit %d: id %d\n",
	//		  ((card *) aux->payload)->suit, ((card *) aux->payload)->id);
	//	aux = aux->next;
	//}

	stack dummy_tail = {NULL, NULL};
	stack *player_cards = &dummy_tail;

	cards_left = give_card(megadeck, &player_cards, cards_left);
	cards_left = give_card(megadeck, &player_cards, cards_left);
	cards_left = give_card(megadeck, &player_cards, cards_left);
	cards_left = give_card(megadeck, &player_cards, cards_left);
	printf("cards_left = %d\n", cards_left);

	stack *aux = player_cards;
	while (aux->next) { // enquanto não chegámos ao dummy tail
		printf("%d: %d\n", ((card *) aux->payload)->suit, ((card *) aux->payload)->id);
		aux = aux->next;
	}

	destroy_megadeck(megadeck);
}

int create_megadeck(list *head, int num_decks)
{
	int total_cards = 0;

	for(int i = 0; i < num_decks; i++)
		for(int j = clubs; j <= spades; j++)
			for(int k = 0; k < SUIT_SIZE; k++) {
				//cur_card = cards[i*DECK_SIZE + j*SUIT_SIZE + k];
				card *cur_card = (card *) calloc(1, sizeof(card));
				cur_card->suit = j;
				cur_card->id = k;
				list_append(head, cur_card);
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

int give_card(list *megadeck, stack **player_cards, int cards_left)
{
	int random = rand() % cards_left;

	list *aux = megadeck->next; // dummy head
	for (int i = 0; i < random; i++) {
		if (aux->next)
			aux = aux->next;
		else
			// o que queremos remover não existe
			exit(EXIT_FAILURE);
	}

	//printf("rand: %d\n", random);
	//printf("suit: %d id: %d\n", ((card *) aux->payload)->suit, ((card *) aux->payload)->id);
	stack_push(player_cards, aux->payload);
	//printf("suit: %d id: %d\n", ((card *) (*player_cards)->payload)->suit, ((card *) (*player_cards)->payload)->id);
	list_remove(aux);
	cards_left--;

	return cards_left;
}
