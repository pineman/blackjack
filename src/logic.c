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

	for (int i = 0; i < MAX_PLAYERS; i++) {
		new_player = (Player *) calloc((size_t) 1, sizeof(Player));
		if (i + 1 <= config->num_players) {
			// Jogadores efetivos
			new_player->type = config->player_type[i];
			new_player->ingame = true;
			strcpy(new_player->name, config->player_names[i]);
			new_player->money = config->money[i];
			new_player->bet = config->bets[i];
		}
		else {
			// Lugar não especificado na configuração.
			strcpy(new_player->name, "Empty");
			new_player->type = VA;
			new_player->ingame = false;
		}
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

int give_card(Player *player, Megadeck *megadeck)
{
	int random = 0;

	if (!megadeck->cards_left)
		megadeck->cards_left = create_megadeck(megadeck);

	// random: 1 - cards_left
	// random é o número de nós a seguir na lista, por isso,
	// tem de ser pelo menos 1 (dummy head node),
	// ou no máximo o número de nós (se seguirmos *cards_left
	// nós a partir do dummy head node, chegamos à tail)

	random = rand() % megadeck->cards_left + 1;
	List *random_card = megadeck->deck;
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
	--megadeck->cards_left;
	return 0;
}

int create_megadeck(Megadeck *megadeck)
{
	int total_cards = 0;
	Card *cur_card = NULL;

	for (int i = 0; i < megadeck->num_decks; i++)
		for (int j = 0; j < 4; j++)
			for (int k = 0; k < SUIT_SIZE; k++) {
				cur_card = (Card *) calloc(1, sizeof(Card));
				cur_card->suit = j;
				cur_card->id = k;
				list_append(megadeck->deck, cur_card);
			}

	total_cards = megadeck->num_decks * DECK_SIZE;
	return total_cards;
}

void test_quit(List *players, bool *quit)
{
    List *aux = find_active_player(players);
    if (aux)
        return;

    *quit = true;
}

void new_game(List *players, Player *house, Megadeck *megadeck)
{
	// só fazer new_game quando já toda a gente jogou
    List *aux = find_active_player(players);
    if (aux)
		return;

	new_game_house(house, megadeck);
	printf("house->points = %d\n", house->points);

	new_game_players(players, house, megadeck);

	if (house->status == BJ)
		// a ronda acaba mais cedo
		pay_bets(players, house);
}

void new_game_house(Player *house, Megadeck *megadeck)
{
	destroy_stack(&house->cards);
	for (int i = 0; i < 2; i++)
		give_card(house, megadeck);
	house->num_cards = 1; // desenhar só uma carta
	house->status = WW;

	count_points(house);
    if (house->points == 21)
		house->status = BJ;
}

void new_game_players(List *players, Player *house, Megadeck *megadeck)
{
    bool found = 0;
    List *aux = players->next;
    Player *cur_player = NULL;
    while (aux) {
		cur_player = (Player *) aux->payload;
		if (cur_player->money < cur_player->bet)
			cur_player->ingame = false;

		// Se o jogador jogou na ronda antes
		if (cur_player->ingame) {
			// Limpar cartas antigas
			destroy_stack(&cur_player->cards);

			// Verificar se o jogador pode jogar outra vez
			if (cur_player->money < cur_player->bet)
				cur_player->ingame = false;
		}

		// se puder jogar...
		if (cur_player->ingame) {
			// Distribuir cartas novas
			for (int i = 0; i < 2; i++)
				give_card(cur_player, megadeck);
			cur_player->num_cards = 2;
			cur_player->status = WW;

			// Contar pontos e verificar se tem blackjack
			count_points(cur_player);
			if (cur_player->points == 21)
				cur_player->status = BJ;

			// Dar a vez ao primeiro jogador sem blackjack
			// E não dar a vez a ninguém se a casa tiver blackjack
			if (!(house->status == BJ) && !(cur_player->status == BJ) && !found) {
				cur_player->playing = true;
				found = true;
			}
			else
				cur_player->playing = false;

			// Retirar as apostas a todos os jogadores
			// (apenas fazemos o cálculo dos dinheiros no final da ronda!)
			cur_player->money -= cur_player->bet;
		}
		aux = aux->next;
    }
}

List *find_active_player(List *players)
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

    return aux;
}

void double_bet(List *players, Player *house, Megadeck *megadeck)
{
    List *aux = find_active_player(players);
    Player *cur_player = (Player *) aux->payload;

    if (cur_player->money < cur_player->bet || cur_player->num_cards > 2)
        return;

    cur_player->money -= cur_player->bet;
    cur_player->bet += cur_player->bet;

    player_hit(players, house, megadeck);
    if (!(cur_player->status == BU))
        stand(players, house, megadeck);
}

void bet(List *players)
{
	char buffer[MAX_PLAYER_NAME+2];
    List *aux = find_active_player(players);
    Player *cur_player = NULL;
    if (aux)
		return;

	aux = players->next;
	printf("Insira o nome do jogador a modificar a aposta: ");
	fgets(buffer, sizeof(buffer), stdin);
	int newline = (int) strcspn(buffer, "\n");

	if (newline == 9) {
		puts("O nome do jogador tem no máximo 8 caracteres.");
		return;
	}
	else
		buffer[newline] = '\0';

	bool found_player = false;
	while (aux && !found_player) {
		cur_player = (Player *) aux->payload;
		if (strcmp(buffer, cur_player->name) == 0 && !found_player)
			found_player = true;
		else
			aux = aux->next;
	}

	if (!aux) {
		puts("Jogador não encontrado.");
		return;
	}

	cur_player = (Player *) aux->payload;
	printf("Insira o novo valor da aposta do jogador %s: ", cur_player->name);
	fgets(buffer, sizeof(buffer), stdin);
	long new_bet = strtol(buffer, NULL, 10);
	// 25% ou 100% do dinheiro?
	// o dinheiro do jogador está (essencialmente) garantido
	// de estar abaixo de INT_MAX (a não ser que se jogue mesmo muito)
	if (new_bet > cur_player->money || new_bet <= 0) {
		puts("Nova aposta inválida.");
		return;
	}

	cur_player->bet = (int) new_bet;
}

void stand(List *players, Player *house, Megadeck *megadeck)
{
	List *aux = find_active_player(players);
	Player *cur_player = NULL;

	// Se não encontrarmos um jogador a jogar...
	if (!aux) {
		// não fazer nada
        return;
    }
    else {
		// se encontrarmos, fazer-lhe stand
		cur_player = (Player *) aux->payload;
		if (cur_player->status == WW)
			cur_player->status = ST;
		cur_player->playing = false;

		// passar ao próximo jogador
        aux = aux->next;
    }

	if (aux) {
		// se este próximo jogador existir,
		// procurar o próximo jogador válido a seguir
		while (aux) {
			cur_player = (Player *) aux->payload;
			if (cur_player->ingame && !(cur_player->status == BJ))
				break;
			else
				aux = aux->next;
		}
		// se ele existir, dar-lhe a vez
		if (aux) {
			cur_player->playing = true;
		}
		else {
			// não existe um próximo jogador válido para jogar
			// TODO: se não existir, é o hit da casa
			house_hit(house, megadeck);
			pay_bets(players, house);
		}
	}
	else {
		// se não existir um próximo jogador, fizemos stand do último jogador
		// TODO: se não existir, é o hit da casa
		house_hit(house, megadeck);
		pay_bets(players, house);
	}
}

void player_hit(List *players, Player *house, Megadeck *megadeck)
{
	List *aux = find_active_player(players);
	Player *cur_player = NULL;

	if (aux)
		cur_player = (Player *) aux->payload;
	else
		return;

	give_card(cur_player, megadeck);
	count_points(cur_player);

	if (cur_player->points > 21)
		cur_player->status = BU;
	if (cur_player->points >= 21)
		stand(players, house, megadeck);
}

void house_hit(Player *house, Megadeck *megadeck)
{
	house->num_cards = 2;

	while (house->points < 17) {
		give_card(house, megadeck);
		count_points(house);
		printf("house->points = %d\n", house->points);
	}

	if (house->points > 21)
		house->status = BU;
}

void pay_bets(List *players, Player *house)
{
    List *aux = players->next;
	Player *cur_player = NULL;
	while (aux) {
        cur_player = ((Player *) aux->payload);

        // not playing
        if (!cur_player->ingame) {
            // skip this player
            aux = aux->next;
            continue;
        }

		// surrender
		if (cur_player->status == SU) {
		 	house->money -= cur_player->bet / 2;
		 	cur_player->money += cur_player->bet / 2;
		    cur_player->losses++;
        }
		// blackjack casa e do jogador: tie
        else if (cur_player->status == BJ && house->status == BJ) {
            cur_player->money += cur_player->bet;
            cur_player->ties++;
        }
        // blackjack do jogador: win
        else if (cur_player->status == BJ && !(house->status == BJ)) {
        	cur_player->money += 2*cur_player->bet + cur_player->bet/2;
            house->money -= cur_player->bet + cur_player->bet/2;
            cur_player->wins++;
        }
        // blackjack da casa: loss
        else if (!(cur_player->status == BJ) && house->status == BJ) {
        	house->money += cur_player->bet;
        	cur_player->losses++;
        }
        // bust da casa e do jogador: loss
        else if (cur_player->status == BU) {
            house->money += cur_player->bet;
            cur_player->losses++;
        }
        // bust da casa: win
        else if (!(cur_player->status == BU) && house->status == BU) {
            cur_player->money += 2*cur_player->bet;
        	house->money -= cur_player->bet;
            cur_player->wins++;
        }
        // empate mesmos pontos: tie
        else if (cur_player->points == house->points) {
            cur_player->money += cur_player->bet;
            cur_player->ties++;
        }
        // jogador ganha com mais pontos: win
        else if (cur_player->points > house->points) {
            cur_player->money += 2*cur_player->bet;
            house->money -= cur_player->bet;
            cur_player->wins++;
        }
        // house ganha com mais pontos: loss
        else if (cur_player->points < house->points) {
        	house->money += cur_player->bet;
        	cur_player->losses++;
        }
        else {
			// TODO: this should never happen
        	// exit(EXIT_FAILURE);
        }
        aux = aux->next;
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
