#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <string.h>
#include <stdbool.h>
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

int init_game(Config *config, List *players)
{
	Player *new_player = NULL;
	const int num_decks = config->num_decks;

	for (int i = 0; i < config->num_players; i++) {
		new_player = (Player *) calloc((size_t) 1, sizeof(Player));
		new_player->ingame = true;
		strcpy(new_player->name, config->player_names[i]);
		new_player->type = config->player_type[i];
		new_player->money = config->money[i];
		new_player->bet = config->bets[i];
		new_player->playing = false;
		list_append(players, new_player);
	}

	return num_decks;
}

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

int give_card(Player *player, List *megadeck, int *cards_left, const int num_decks)
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

int create_megadeck(List *megadeck, const int num_decks)
{
	int total_cards = 0;
	Card *cur_card = NULL;

	for (int i = 0; i < num_decks; i++)
		for (int j = 0; j < 4; j++)
			for (int k = 0; k < SUIT_SIZE; k++) {
				cur_card = (Card *) calloc(1, sizeof(Card));
				cur_card->suit = j;
				cur_card->id = k;
				list_append(megadeck, cur_card);
			}

	total_cards = num_decks * DECK_SIZE;
	return total_cards;
}

void new_game(List *players, Player *house, List *megadeck, int *cards_left, const int num_decks)
{
	bool found = false;
    List *aux = players->next;
    Player *cur_player = NULL;
    while (aux) {
		cur_player = (Player *) aux->payload;
		if (cur_player->playing) {
			found = true;
			break;
		}
		aux = aux->next;
    }
    if (found)
		return;

	new_game_house(house, megadeck, cards_left, num_decks);

	new_game_players(players, house, megadeck, cards_left, num_decks);
}

void new_game_house(Player *house, List *megadeck, int *cards_left, const int num_decks)
{
	destroy_stack(&house->cards);
	for (int i = 0; i < 2; i++)
		give_card(house, megadeck, cards_left, num_decks);
	house->num_cards = 2;
	house->status = WW;

	count_points(house);
    if (house->points == 21)
		house->status = BJ;
}

void new_game_players(List *players, Player *house, List *megadeck, int *cards_left, const int num_decks)
{
    bool found = 0;
    List *aux = players->next;
    Player *cur_player = NULL;
    while (aux) {
		cur_player = (Player *) aux->payload;
		if (cur_player->money < cur_player->bet)
			cur_player->ingame = false;

		if (cur_player->ingame) {
			destroy_stack(&cur_player->cards);
			for (int i = 0; i < 2; i++)
				give_card(cur_player, megadeck, cards_left, num_decks);
			cur_player->num_cards = 2;
			cur_player->status = WW;

			count_points(cur_player);
			if (cur_player->points == 21)
				cur_player->status = BJ;

			if (!(house->status == BJ) && !(cur_player->status == BJ) && !found) {
				cur_player->playing = true;
				found = true;
			}
			else
				cur_player->playing = false;

			cur_player->money -= cur_player->bet;
		}
		aux = aux->next;
    }
}

void stand(List *players, Player *house, List *megadeck, int *cards_left, const int num_decks)
{
	List *aux = players->next; // dummy head
	Player *cur_player = NULL;
	while (aux) {
		// iterar até ao jogador que está a jogar
		cur_player = (Player *) aux->payload;
		if (cur_player->playing)
			break;
		else
			aux = aux->next;
	}

	// fazer-lhe stand
	cur_player->status = ST;
	cur_player->playing = false;

	// Passar ao próximo jogador
	if (aux) aux = aux->next;

	if (aux) {
		// se ele existir, procurar o próximo jogador válido a seguir
		while (aux) {
			cur_player = (Player *) aux->payload;
			if (cur_player->ingame && !(cur_player->status == BJ))
				break;
			else
				aux = aux->next;
		}
		// se ele existir, dar-lhe a vez
		if (aux) cur_player->playing = true;
	}
	else {
		// TODO: se não existir, é o hit da casa
	}
}

void count_points(Player *player)
{
    Stack *cards = player->cards;
    int num_ace = 0;

	player->points = 0;
    while (cards) {
        player->points += point_index(cards->card->id);
        if (cards->card->id == 12)
            num_ace++;
        cards = cards->next;
	}

    while (player->points > 21 && num_ace > 0) {
        player->points -= 10;
        --num_ace;
    }
}

int point_index(int id)
{
    int points = 0;
    id %= 13;
    if (id == 12)
        points = 11;
    else if (id <12 && id>8)
        points = 10;
    else
        points = id + 2;
    return points;
}

void pay_bets(list *players, player *house)
{
    list *aux = players->next;
	while (aux) {
		cur_player = ((player *) aux->payload);
        
        // blackjack casa e do jogador
        if (cur_player->status == bj && house->status = bj)
            cur_player->money += cur_player->bet;
        // blackjack do jogador
        else if (cur_player->status == bj)
            cur_player->money += 2*cur_player->bet + cur_player/2;
        // bust da casa e do jogador
        else if (cur_player->status == bu && house->status == bu)
            cur_player->money += cur_player->bet; 
        // bust da casa
        else if (house->status == bu)
            cur_player->money += 2*cur_player->bet;
        // empate mesmos pontos
        else if (cur_player->points == house->points)
            cur_player->money += cur_player->bet;
        // jogador ganha com mais pontos
        else if (cur_player->points > house->points)
            cur_player->money += 2*cur_player->bet;     
    }
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
		free(stack_pop(cards));
}

