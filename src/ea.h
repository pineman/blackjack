#ifndef EA_H
#define EA_H

#include <stdbool.h>
#include "logic.h"

#define COLUMNS	10
#define HARD_LINES 10
#define SOFT_LINES 7

void write_matrix(Move ***matrix, FILE *file, int lines);
void destroy_matrix(Move **matrix, int lines);
Strategy *read_strategy(char *filename);
Move get_decision(Player *player, Card *house_card, Strategy *strategy);
void ea_make_decision(List *players, Player *house, Megadeck *megadeck, Strategy *strategy);
int count_strategy(Player *player, Card *new_card, int count);

#endif
