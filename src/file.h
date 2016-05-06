#ifndef FILE_H
#define FILE_H

#include "logic.h"
#define MAX_LINE_LEN 100

Config *read_player(char *line, Config *game_config, int count);
Config *read_config(char * filename);

#endif
