#ifndef FILE_H
#define FILE_H

#include "logic.h"
#define MAX_LINE_LEN 100

Config *read_player(char *line, Config *game_config, int count);
Config *read_config(char * filename);
int get_new_bet(List *players);
Player *get_new_player(int pos);
void write_stats(List *players, Player *house, List *old_players, bool *quit);
void write_stats_players(FILE *stats, List *players);

#endif
