#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <string.h>
#include <stdbool.h>
#include "logic.h"
#include "list.h"
#include "file.h"
#include "sdl.h"
#include "ea.h"
#include "error.h"

// Definição das operações válidas nas pilhas de cartas
void stack_push(Stack **sp, Card *card)
{
	Stack *old_sp = *sp;
	Stack *new = (Stack *) ecalloc((size_t) 1, sizeof(Stack));

	new->card = card;

	new->next = old_sp;
	new->prev = NULL;
	if (old_sp != NULL)
		old_sp->prev = new;
	else {
		// estamos a pushar o primeiro elemento, sp tava a NULL
	}

	*sp = new;
}

Card *stack_pop(Stack **sp)
{
	if (*sp == NULL) {
		puts("Erro: tentou-se fazer pop numa stack vazia.");
		exit(EXIT_FAILURE);
	}

	Stack *pop = *sp;
	Card *card = pop->card;
	*sp = pop->next;
	if (pop->next != NULL)
		pop->next->prev = NULL;

	free(pop);

	return card;
}

int init_game(Config *config, List *players)
{
	Player *new_player = NULL;
	const int num_decks = config->num_decks;

	for (int i = 0; i < MAX_PLAYERS; i++) {
		new_player = (Player *) ecalloc((size_t) 1, sizeof(Player));
		if (i + 1 <= config->num_players) {
			// Jogadores efetivos
			new_player->type = config->player_type[i];
			new_player->ingame = true;
			strcpy(new_player->name, config->player_names[i]);
			new_player->money = config->money[i];
			new_player->orig_bet = config->bets[i];
			new_player->bet = new_player->orig_bet;
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

	free(config);

	return num_decks;
}

int give_card(Player *player, Megadeck *megadeck)
{
	int random = 0;

	if (megadeck->cards_left == 0)
		megadeck->cards_left = create_megadeck(megadeck);

	// random: 1 - cards_left
	// random é o número de nós a seguir na lista, por isso,
	// tem de ser pelo menos 1 (dummy head node),
	// ou no máximo o número de nós (se seguirmos *cards_left
	// nós a partir do dummy head node, chegamos à tail)
	random = rand() % megadeck->cards_left + 1;
	List *random_card = megadeck->deck;
	for (int i = 0; i < random; i++) {
		if (random_card->next != NULL)
			random_card = random_card->next;
		else {
			puts("Erro: tentou-se dar uma carta não existente.");
			exit(EXIT_FAILURE);
		}
	}

	count_strategy(player, (Card *) random_card->payload, megadeck);

	stack_push(&(player->cards), random_card->payload);
	player->num_cards++;
	list_remove(random_card);
	megadeck->cards_left--;
	return 0;
}

int create_megadeck(Megadeck *megadeck)
{
	int total_cards = 0;
	Card *cur_card = NULL;

	for (int i = 0; i < megadeck->num_decks; i++)
		for (int j = 0; j < 4; j++)
			for (int k = 0; k < SUIT_SIZE; k++) {
				cur_card = (Card *) ecalloc(1, sizeof(Card));
				cur_card->suit = j;
				cur_card->id = k;
				list_append(megadeck->deck, cur_card);
			}

	total_cards = megadeck->num_decks * DECK_SIZE;
	return total_cards;
}

void new_game(List *players, Player *house, Megadeck *megadeck)
{
	// só fazer new_game quando já toda a gente jogou
    List *aux = find_active_player(players);
    if (aux != NULL) {
		return;
	}

	clear_cards(players, house);

    // só fazer new_game quando houver jogadores para jogar
    aux = find_ingame_player(players);
    if (aux == NULL) {
        return;
    }

	distribute_cards(players, house, megadeck);
	find_playing(players, house);

	if (house->status == BJ) {
		// a ronda acaba mais cedo
		pay_bets(players, house);
		return;
	}

	hi_lo(players, megadeck);
}

void clear_cards(List *players, Player *house)
{
    List *aux = players->next;
    Player *cur_player = NULL;
    while (aux != NULL) {
		cur_player = (Player *) aux->payload;
		if (cur_player->money < cur_player->bet)
			cur_player->ingame = false;

		// Se o jogador jogou na ronda antes
		if (cur_player->ingame) {
			// Limpar cartas antigas
			destroy_stack(&cur_player->cards);
			cur_player->num_cards = 0;

			// Verificar se o jogador pode jogar outra vez
			if (cur_player->money < cur_player->bet)
				cur_player->ingame = false;
		}
		aux = aux->next;
	}
	destroy_stack(&house->cards);
}

void distribute_cards(List *players, Player *house, Megadeck *megadeck)
{
	List *aux = players->next;
	Player *cur_player = NULL;
	// Distribuir cartas realisticamente
    for (int i = 0; i < 2; i++) {
		aux = players->next;
		cur_player = NULL;
		while (aux != NULL) {
			cur_player = (Player *) aux->payload;
			// se puder jogar...
			if (cur_player->ingame)
				// dar uma carta
				give_card(cur_player, megadeck);
			aux = aux->next;
		}
		give_card(house, megadeck);
	}
}

// esta função vê quem tem blackjack e dá a vez ao primeiro jogador sem
void find_playing(List *players, Player *house)
{
	house->num_cards = 1; // desenhar só uma carta
	house->status = WW;

	count_points(house);
	// TODO: desenhar os pontos da casa
	printf("\n\nhouse->points = %d\n\n", house->points);
    if (house->points == 21)
		house->status = BJ;

    bool found = false;
    List *aux = players->next;
    Player *cur_player = NULL;
	while (aux != NULL) {
		cur_player = (Player *) aux->payload;
		if (cur_player->ingame) {
			// Colocar status a Waiting
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

List *find_ingame_player(List *players)
{
	List *aux = players->next; // dummy head
	Player *cur_player = NULL;
	while (aux != NULL) {
		// iterar até ao jogador que está a jogar
		cur_player = (Player *) aux->payload;
		if (cur_player->ingame)
			break;
		else
			aux = aux->next;
    }

    return aux;
}

List *find_active_player(List *players)
{
	List *aux = players->next; // dummy head
	Player *cur_player = NULL;
	while (aux != NULL) {
		// iterar até ao jogador que está a jogar
		cur_player = (Player *) aux->payload;
		if (cur_player->playing)
			break;
		else
			aux = aux->next;
    }

    return aux;
}

/*
 * Esta função é chamada na main apenas para
 * registar as teclas de hit, stand, etc. se o jogador
 * for humano (ou seja para as teclas não afetarem as
 * jogadas dos jogadores EA mesmo se primidas)
 */
List *find_active_human_player(List *players)
{
	List *aux = players->next; // dummy head
	Player *cur_player = NULL;
	while (aux != NULL) {
		// iterar até ao jogador que está a jogar
		cur_player = (Player *) aux->payload;
		if (cur_player->playing && cur_player->type == HU)
			break;
		else
			aux = aux->next;
    }

    return aux;
}

void surrender(List *players, Player *house, Megadeck *megadeck)
{
    List *aux = find_active_player(players);
    if (aux == NULL) {
		return;
	}

    Player *cur_player = (Player *) aux->payload;

    cur_player->status = SU;
    stand(players, house, megadeck);
}

bool double_bet(List *players, Player *house, Megadeck *megadeck)
{
    List *aux = find_active_player(players);
    // não fazer nada se não for a vez dum jogador
    if (aux == NULL) {
        return false;
    }

    Player *cur_player = (Player *) aux->payload;

	// não fazer nada se o jogador não pode fazer double
    if (cur_player->money < cur_player->bet || cur_player->num_cards != 2)
        return false;

    cur_player->money -= cur_player->bet;
    cur_player->bet += cur_player->bet;

    player_hit(players, house, megadeck);
    if (!(cur_player->status == BU)) {
        stand(players, house, megadeck);
	}

	return true;
}

void bet(List *players)
{
    List *aux = find_active_player(players);
    if (aux != NULL) {
		return;
	}

	get_new_bet(players);
}

AddPlayerError add_player(List *players, List *old_players, SDL_Window *window)
{
	int pos = get_clicked_player();
	if (pos == 0) {
		return OUT;
	}

	List *aux = list_follow(players, pos);
	Player *old_player = (Player *) aux->payload;
	if (old_player->ingame) {
		return NOTEMPTY;
	}

	show_add_player_input_message(window);

	Player *new_player = get_new_player(pos);

	old_player = (Player *) list_remove_pos(players, pos);
	list_append(old_players, old_player);
	list_insert_pos(players, pos, new_player);

	return OK;
}

void stand(List *players, Player *house, Megadeck *megadeck)
{
	List *aux = find_active_player(players);
	Player *cur_player = NULL;
	bool end_of_round = false;

	// Se não encontrarmos um jogador a jogar...
	if (aux == NULL) {
		// não fazer nada
        return;
    }

	// se encontrarmos, fazer-lhe stand
	cur_player = (Player *) aux->payload;
	if (cur_player->status == WW)
		cur_player->status = ST;
	cur_player->playing = false;

	// passar ao próximo jogador
	aux = aux->next;

	if (aux != NULL) {
		// se este próximo jogador existir,
		// procurar o próximo jogador válido a seguir
		while (aux != NULL) {
			cur_player = (Player *) aux->payload;
			if (cur_player->ingame && !(cur_player->status == BJ))
				break;
			else
				aux = aux->next;
		}
		// se ele existir, dar-lhe a vez
		if (aux != NULL) {
			cur_player->playing = true;
		}
		else {
			// não existe um próximo jogador válido para jogar
			end_of_round = true;
		}
	}
	else {
		// se não existir um próximo jogador, fizemos stand do último jogador
		end_of_round = true;
	}

	if (end_of_round) {
		house_hit(house, megadeck);
	    pay_bets(players, house);
	}
}

void player_hit(List *players, Player *house, Megadeck *megadeck)
{
	List *aux = find_active_player(players);
	Player *cur_player = NULL;

	if (aux != NULL) {
		cur_player = (Player *) aux->payload;
	}
	else {
		return;
	}

	give_card(cur_player, megadeck);
	count_points(cur_player);

	if (cur_player->points > 21) {
		cur_player->status = BU;
	}

	if (cur_player->points >= 21) {
		stand(players, house, megadeck);
	}
}

void house_hit(Player *house, Megadeck *megadeck)
{
	house->num_cards = 2;

	while (house->points < 17) {
		give_card(house, megadeck);
		count_points(house);
		// TODO: desenhar os pontos da casa
		printf("\n\nhouse->points = %d\n\n", house->points);
	}

	if (house->points > 21) {
		house->status = BU;
	}
}

void pay_bets(List *players, Player *house)
{
    List *aux = players->next;
	Player *cur_player = NULL;
	while (aux != NULL) {
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
			// isto nunca pode acontecer
			puts("Erro: estado de jogador desconhecido.");
        	exit(EXIT_FAILURE);
        }
        aux = aux->next;
    }
}

void count_points(Player *player)
{
    Stack *cards = player->cards;
    int num_ace = 0;

	player->points = 0;
    while (cards != NULL) {
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
	while (aux != NULL) {
		tmp = aux;
		aux = tmp->next;
		free(tmp->payload);
		free(tmp);
	}
	free(head);
}

void destroy_stack(Stack **cards)
{
	while (*cards != NULL) {
		free(stack_pop(cards));
	}
}

void destroy_players_list(List *players)
{
    List *aux = players->next;
    Player *cur_player = NULL;
    while (aux != NULL) {
		cur_player = (Player *) aux->payload;
		destroy_stack(&cur_player->cards);
		aux = aux->next;
    }
	destroy_list(players);
}
