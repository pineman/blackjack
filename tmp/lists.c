/*
 * Proof of concept das listas que vamos usar no projeto.
 * Estas são listas de payload genérico,
 * doubly-linked com dummy head node.
 *
 * A desvantagem de utilizar listas de payload genérico é que
 * obrigamos o utilizador a fazer cast do payload para o que ele
 * precisar, o que pode ser chato, adicionar complexidade ao código
 * e/ou criar bugs difíceis de perceber se nos esquecermos de
 * fazer cast da payload... (ou seja, fazia-se dereference dum
 * void pointer, o que é ilegal).
 *
 * Mas, por outro lado, o programa torna-se mais modular,
 * mais fácil de compreender e abstrai-se assim toda a parte
 * das listas.
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

// estes não ficam aqui!!
#define	DECK_SIZE	52
#define SUIT_SIZE	13

enum suits {clubs, diamonds, hearts, spades};

// Doubly-linked list with dummy head nodes
typedef struct list {
	void *payload; // payload genérica
	struct list *next;
	struct list *prev;
} list;

typedef struct card {
	enum suits suit;
	int id; // 0 a 12
} card;

typedef struct player {
	int position;
	char name[8];
} player;

void list_prepend(list *head, void *payload);
void list_append(list *head, void *payload);
bool list_remove(list *head, int pos);
int create_megadeck(list *head, int num_decks);

int main()
{
	list dummy = {NULL, NULL, NULL};
	list *head = &(dummy);
	int cards_left = 0;

	cards_left = create_megadeck(head, 2);
	printf("%d\n", cards_left);

	list *aux = head;
	while (aux->next) {
		aux = aux->next;
		printf("suit %d: id %d\n",
			  ((card *) aux->payload)->suit, ((card *) aux->payload)->id);
		free(aux->payload);
		free(aux);
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

bool list_remove(list *head, int pos)
{
	list *aux = head;
	for (int i = 0; i < pos; i++) {
		if (aux->next)
			aux = aux->next;
		else
			// o que queremos remover não existe
			return false;
	}
	list *to_rm = aux;
	to_rm->next->prev = to_rm->prev;
	to_rm->prev->next = to_rm->next;
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
