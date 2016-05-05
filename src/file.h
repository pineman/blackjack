#ifndef FILE_H
#define FILE_H

#include "logic.h"
#define MAX_LINE_LEN 100

typedef struct Config {
	int num_decks;
	int num_players;
	int player_type[MAX_PLAYERS];
	char player_names[MAX_PLAYERS][MAX_PLAYER_NAME+1];
	int money[MAX_PLAYERS];
	int bets[MAX_PLAYERS];
} Config;

Config *read_config(char * filename);

#endif
