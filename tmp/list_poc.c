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
#include "list.h"

// estes não ficam aqui!!
#define	DECK_SIZE	52
#define SUIT_SIZE	13

enum suits {clubs, diamonds, hearts, spades};

typedef struct card {
	enum suits suit;
	int id; // 0 a 12
} card;

int create_megadeck(list *head, int num_decks);

int main()
{
	list dummy = {NULL, NULL, NULL};
	list *head = &dummy;
	int cards_left = 0;

	cards_left = create_megadeck(head, 2);
	printf("%d\n", cards_left);

	list *aux = head->next;
	list *tmp = NULL;
	while (aux) {
		tmp = aux;
		printf("suit %d: id %d\n",
			  ((card *) tmp->payload)->suit, ((card *) tmp->payload)->id);
		aux = tmp->next;
		free(tmp->payload);
		free(tmp);
	}
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
