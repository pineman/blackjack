#ifndef EA_H
#define EA_H

#include <stdbool.h>
#include "logic.h"

#define MAX_LINE_SIZE 12

void write_matrix(Move ***matrix, FILE *fp);
Strategy *read_strategy();
Move get_decision(Player *player, Card *house_card, Strategy *strategy);
bool make_decision(List *players, Player *house, Megadeck *megadeck, Strategy *strategy);
void ea_play(List *players, Player *house, Megadeck *megadeck, Strategy *strategy);

#endif
