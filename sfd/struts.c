#include <stdio.h>
#include <stdlib.h>

enum naipes {paus, ouros, copas, espadas}

typedef struct card{
	naipes naipe;
	int value;
	card *next;
} card;

typedef struct 