#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "list.h"
#include "structs.h"
#include "poc.h"

void print_list(list *megadeck);

int main()
{
	srand(time(NULL));

	list dummy_head = {NULL, NULL, NULL};
	list *megadeck = &dummy_head;
	int cards_left = 0;
	int num_decks = 2;

	player pineman = player_default;

	for (int j = 0; j < 10; j++) {
		for (int i = 0; i < 11; i++) {
			give_card(megadeck, &cards_left, num_decks, &pineman);
			printf("suit: %d id: %d\n", pineman.cards[i]->suit, pineman.cards[i]->id);
		}
		printf("cards_left = %d\n", cards_left);
		destroy_player_cards(&pineman);
	}

	destroy_megadeck(megadeck);
}

void print_list(list *megadeck)
{
	list *aux = megadeck;
	int i = 1;
	while (aux) {
		printf("elements = %d ", i++);
		printf("node: %p, node->prev: %p, node->next: %p\n", aux, aux->prev, aux->next);
		if (aux->payload)
			printf("suit: %d id: %d\n", ((card *) aux->payload)->suit, ((card *) aux->payload)->id);
		aux = aux->next;
	}
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

void destroy_player_cards(player *player)
{
	for (int i = 0; i < player->num_cards; i++)
		free(player->cards[i]);
	player->num_cards = 0;
}

int give_card(
	list *megadeck, int *cards_left, int num_decks,
	player *player)
{
	int random = 0;

	if (player->num_cards == MAX_CARD_HAND)
		// TODO: o jogador não pode receber mais cartas
		return -1;

	if (!*cards_left)
		*cards_left = create_megadeck(megadeck, num_decks);

	random = rand() % (*cards_left + 1) + 1;
	// random: 1 - cards_left
	// random é o número de nós a seguir na lista, por isso,
	// tem de ser pelo menos 1 (dummy head node),
	// ou no máximo o número de nós (se seguirmos *cards_left
	// nós a partir do dummy head node, chegamos à tail)
	random = *cards_left;

	list *random_node = megadeck;
	for (int i = 0; i < random; i++) {
		if (random_node->next)
			random_node = random_node->next;
		else
			// TODO: a carta que queremos dar não existe
			return -1;
	}

	player->cards[player->num_cards] = (card *) random_node->payload;
	player->num_cards++;
	list_remove(random_node);
	--*cards_left;
	return 0;
}
