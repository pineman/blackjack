#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "list.h"
#include "stack.h"
#include "structs.h"
#include "poc.h"

/*
 * roadmap:
 * dar cartas duma lista de cartas
 * a uma stack apontada pela struct jogador
 * que está dentro duma lista de jogadores
 *
 * kek
 */

int main()
{
	srand(time(NULL));

	// generate 'fake' data
	// in the real program, these will be initialized once the game begins
	// init megadeck and players list
	list dummy_head_megadeck = default_dummy_head;
	list dummy_head_players = default_dummy_head;
	list *megadeck = &dummy_head_megadeck;
	list *players = &dummy_head_players;

	// init config
	int cards_left = 0;
	int num_decks = 2;

	// init player
	player *pineman = (player *) calloc((size_t) 1, sizeof(player));
	// append him to the players list
	list_append(players, pineman);

	// give_card will populate the megadeck automatically when its called
	// since cards_left = 0

	// this var is not directly reusable because the stack pointer
	// will change all the time, so we need to fetch the current version
	// using the same big ass expression...
	stack *player_cards = ((player *) players->next->payload)->cards;
	printf("stack: %p\n\n", player_cards);

	for (int i = 0; i < 11; i++) {
		give_card(megadeck, &cards_left, num_decks, (player *) players->next->payload);
		// ...right here.
		player_cards = ((player *) players->next->payload)->cards;
		printf("stack: %p\n", player_cards);
		printf("got suit: %d id: %d\n",
			  ((card *) player_cards->payload)->suit,
			  ((card *) player_cards->payload)->id);
		printf("cards_left = %d\n\n", cards_left);
	}

	print_reverse_stack(((player *) players->next->payload)->cards);

	destroy_stack(&player_cards);
	destroy_list(players);
	destroy_list(megadeck);
	// WE LOVE POINTERS!
	// I CANT HEAR YOU!
	// WE LOVE POINTERS!!!
}

int give_card(list *megadeck, int *cards_left, int num_decks, player *player)
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

	list *random_node = megadeck;
	for (int i = 0; i < random; i++) {
		if (random_node->next)
			random_node = random_node->next;
		else
			// TODO: a carta que queremos dar não existe
			return -1;
	}

	stack_push(&(player->cards), random_node->payload);
	printf("given suit: %d id: %d\n", ((card *) random_node->payload)->suit, ((card *) random_node->payload)->id);
	player->num_cards++;
	list_remove(random_node);
	--*cards_left;
	return 0;
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

void destroy_list(list *head)
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

void destroy_stack(stack **cards)
{
	while (*cards) {
		printf("stack: %p\n", *cards);
		free(stack_pop(cards)); // remover carta da lista e a carta
	}
	printf("stack: %p\n", *cards);
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

void print_reverse_stack(stack *sp)
{
	stack *aux = sp;
	stack *tmp = NULL;

	while (tmp != sp) {
		aux = sp;
		while (aux->next != tmp)
			aux = aux->next;
		printf("suit: %d id: %d\n", ((card *) aux->payload)->suit, ((card *) aux->payload)->id);
		tmp = aux;
	}
}
