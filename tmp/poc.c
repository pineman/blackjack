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
	List *megadeck = (List *) calloc((size_t) 1, sizeof(List));
	List *players = (List *) calloc((size_t) 1, sizeof(List));

	// init config
	int cards_left = 0;
	int num_decks = 2;

	// init player
	Player *pineman = (Player *) calloc((size_t) 1, sizeof(Player));
	// append him to the players list
	list_append(players, pineman);

	// give_card will populate the megadeck automatically when its called
	// since cards_left = 0

	// this var is not directly reusable because the stack pointer
	// will change all the time, so we need to fetch the current version
	// using the same big ass expression...
	Stack *player_cards = ((Player *) players->next->payload)->cards;
	printf("stack: %p\n\n", player_cards);

	for (int i = 0; i < 11; i++) {
		give_card(megadeck, &cards_left, num_decks, (Player *) players->next->payload);
		// ...right here.
		player_cards = ((Player *) players->next->payload)->cards;
		printf("stack: %p\n", player_cards);
		printf("got suit: %d id: %d\n",
			  player_cards->card->suit,
			  player_cards->card->id);
		printf("cards_left = %d\n\n", cards_left);
	}

	print_reverse_stack(((Player *) players->next->payload)->cards);

	destroy_stack(&player_cards);
	destroy_list(players);
	destroy_list(megadeck);
	// WE LOVE POINTERS!
	// I CANT HEAR YOU!
	// WE LOVE POINTERS!!!
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
	printf("given suit: %d id: %d\n", ((Card *) random_card->payload)->suit, ((Card *) random_card->payload)->id);
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
	while (*cards) {
		printf("stack: %p\n", *cards);
		free(stack_pop(cards)); // remover carta da lista e a carta
	}
	printf("stack: %p\n", *cards);
}

void print_list(List *megadeck)
{
	List *aux = megadeck;
	int i = 1;
	while (aux) {
		printf("elements = %d ", i++);
		printf("node: %p, node->prev: %p, node->next: %p\n", aux, aux->prev, aux->next);
		if (aux->payload)
			printf("suit: %d id: %d\n", ((Card *) aux->payload)->suit, ((Card *) aux->payload)->id);
		aux = aux->next;
	}
}

void print_reverse_stack(Stack *sp)
{
	Stack *aux = sp;
	Stack *tmp = NULL;

	while (tmp != sp) {
		aux = sp;
		while (aux->next != tmp)
			aux = aux->next;
		printf("suit: %d id: %d\n", aux->card->suit, aux->card->id);
		tmp = aux;
	}
}
