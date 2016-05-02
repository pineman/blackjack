#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// estes não ficam aqui!!
#define	MAX_DECK_SIZE	52
#define MAX_CARD_ID		13

enum suits {clubs=1, diamonds, hearts, spades};

// Doubly-linked list with dummy head nodes
typedef struct list {
	void *payload;
	struct list *next;
	struct list *prev;
} list;

typedef struct card {
	enum suits suit;
	int id;
} card;

typedef struct player {
	int position;
	char name[8];
} player;

void create_megadeck(list *head, int num_decks);
void list_append(list *head, void *payload);
void list_prepend(list *head, void *payload);

int main()
{
	list dummy = {NULL, NULL, NULL};
	list *head = &(dummy);

	create_megadeck(head, 2);

	list *aux = head;
	while (aux->next) {
		printf("%d\n", ((card *) aux->next->payload)->id);
		aux = aux->next;
	}
}

void list_prepend(list *head, void *payload)
{
	list *new_head = (list *) calloc((size_t) 1, sizeof(list));

	new_head->payload = payload;

	new_head->next = head->next;
	new_head->prev = head;
	head->next = new_head;
}

void list_append(list *head, void *payload)
{
	list *aux = head;
	while(aux->next)
		aux = aux->next;
	list *tail = aux;

	list *new_tail = (list *) calloc((size_t) 1, sizeof(list));

	new_tail->payload = payload;

	new_tail->next = NULL;
	new_tail->prev = tail;
	tail->next = new_tail;
}

void create_megadeck(list *head, int num_decks)
{
	for(int i=1; i<=num_decks; i++) {
		printf("num_decks = %d\n", i);

		for(int j=clubs; j<spades; j++) {
			printf("suit = %d\n", j);

			for(int k=1; k < MAX_CARD_ID; k++) {
				printf("card_id = %d\n", k);
				card *cur_card = (card *) calloc(1, sizeof(card));

				cur_card->id = k;
				cur_card->suit = j;

				printf("%d: %d", cur_card->suit, cur_card->id);
				list_append(head, cur_card);
			}
		}
	}
}
