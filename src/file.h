#ifndef FILE_H
#define FILE_H

#include "logic.h"
#define MAX_LINE_LEN 100

Config *read_player(char *line, Config *game_config, int count);
Config *read_config(char *filename);

void get_line(char buffer[MAX_PLAYER_NAME+2]);
void get_new_bet(List *players);

Player *get_new_player(int pos);

void write_stats(List *players, Player *house, List *old_players);
void write_stats_players(FILE *stats, List *players);

#endif
