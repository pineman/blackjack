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

void alloc_payload(list **node, void *payload, size_t size)
{
	(*node)->payload = calloc((size_t) 1, size);
	memcpy((*node)->payload, payload, size);
}

void list_prepend(list *head, void *payload, size_t size)
{
	list *new_node = (list *) calloc((size_t) 1, sizeof(list));

	alloc_payload(&new_node, payload, size);
	head->next = new_node;
}

void list_append(list *head, void *payload, size_t size)
{
	while(head->next)
		head = head->next;

	list *new_node = (list *) calloc((size_t) 1, sizeof(list));

	alloc_payload(&new_node, payload, size);

	head->next = new_node;
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
				list_append(head, cur_card, sizeof(card));
			}
		}
	}
}


int main()
{
	list dummy;
	list *head = &(dummy);

	card cur_card = {clubs, 3};
	list_prepend(head, &(cur_card), sizeof(card));
	printf("%d\n", ((card *) head->payload)->id);
}
