#include <stdlib.h>
#include <time.h>
#include "logic.h"
#include "list.h"

/*
 * roadmap:
 * dar cartas duma lista de cartas
 * a uma stack apontada pela struct jogador
 * que está dentro duma lista de jogadores
 *
 * kek
 */

void stack_push(Stack **sp, Card *card)
{
	Stack *old = *sp;
	Stack *new = (Stack *) calloc((size_t) 1, sizeof(Stack));
	new->next = old;
	new->card = card;
	*sp = new;
}

Card *stack_pop(Stack **sp)
{
	if (!*sp)
		// TODO: trying to remove NULL stack
		return NULL;

	Stack *pop = *sp;
	Card *card = pop->card;
	*sp = pop->next;
	free(pop);

	return card;
}

int give_card(List *megadeck, int *cards_left, int num_decks, Player *player)
{
	int random = 0;

	if (player->num_cards == MAX_CARD_HAND)
		// TODO: o jogador não pode receber mais cartas
		return -1;

	if (!*cards_left)
		*cards_left = create_megadeck(megadeck, num_decks);

	// random: 1 - cards_left
	// random é o número de nós a seguir na lista, por isso,
	// tem de ser pelo menos 1 (dummy head node),
	// ou no máximo o número de nós (se seguirmos *cards_left
	// nós a partir do dummy head node, chegamos à tail)
	random = rand() % *cards_left + 1;

	List *random_card = megadeck;
	for (int i = 0; i < random; i++) {
		if (random_card->next)
			random_card = random_card->next;
		else
			// TODO: a carta que queremos dar não existe
			return -1;
	}

	stack_push(&(player->cards), random_card->payload);
	player->num_cards++;
	list_remove(random_card);
	--*cards_left;
	return 0;
}

int create_megadeck(List *megadeck, int num_decks)
{
	int total_cards = 0;

	for(int i = 0; i < num_decks; i++)
		for(int j = clubs; j <= spades; j++)
			for(int k = 0; k < SUIT_SIZE; k++) {
				//cur_card = cards[i*DECK_SIZE + j*SUIT_SIZE + k];
				Card *cur_card = (Card *) calloc(1, sizeof(Card));
				cur_card->suit = j;
				cur_card->id = k;
				list_append(megadeck, cur_card);
			}

	total_cards = num_decks * DECK_SIZE;
	return total_cards;
}

void destroy_list(List *head)
{
	List *aux = head->next; // dummy head
	List *tmp = NULL;
	while (aux) {
		tmp = aux;
		aux = tmp->next;
		free(tmp->payload);
		free(tmp);
	}
	free(head);
}

void destroy_stack(Stack **cards)
{
	while (*cards)
		free(stack_pop(cards)); // remover carta da lista e a carta
}
