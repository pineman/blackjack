#ifndef LOGIC_H
#define LOGIC_H

#include <stdbool.h>
#include <SDL2/SDL.h>
#include "types.h"
#include "list.h"

#define MAX_LINE_LENGTH 64
#define	DECK_SIZE 52
#define SUIT_SIZE 13

int init_game(Config *config, List *players);

void stack_push(Stack **sp, Card *card);
Card *stack_pop(Stack **sp);

int give_card(Player *player, Megadeck *megadeck);
int create_megadeck(Megadeck *megadeck);

void new_game(List *players, Player *house, Megadeck *megadeck);
void clear_cards(List *players, Player *house);
void distribute_cards(List *players, Player *house, Megadeck *megadeck);
void find_playing(List *players, Player *house);

List *find_ingame_player(List *players);
List *find_active_player(List *players);

void surrender(List *players, Player *house, Megadeck *megadeck);
bool double_bet(List *players, Player *house, Megadeck *megadeck);
void bet(List *players);
AddPlayerError add_player(List *players, List *old_players, SDL_Window *window);
void stand(List *players, Player *house, Megadeck *megadeck);
void player_hit(List *players, Player *house, Megadeck *megadeck);
void house_hit(Player *house, Megadeck *megadeck);

void pay_bets(List *players, Player *house);

void count_points(Player *player);
int point_index(int id);

void destroy_list(List *head);
void destroy_stack(Stack **cards);
void destroy_players_list(List *players);

#endif
