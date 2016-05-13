#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "logic.h"
#include "ea.h"

void write_matrix(Move ***matrix, FILE *fp)
{ 
    char buffer[MAX_LINE_SIZE] = {0};
    for (int i=0; fgets(buffer, MAX_LINE_SIZE, fp); i++) {
        *matrix = (Move **) realloc(*matrix, (i+1) * sizeof(Move *));
        (*matrix)[i] = (Move *) calloc(10, sizeof(Move));                
         for (int j=0; j<10; j++) {
            (*matrix)[i][j] =  buffer[j];
        }
    }
}

Strategy *read_strategy()
{
    FILE *fp1 = NULL;
    FILE *fp2 = NULL;
    
    fp1 = fopen("txt/hardtotals.txt", "r");
    if (!fp1) {
        puts("Erro: impossivel ler ficheiro 'hardtotals.txt'");
        exit(EXIT_FAILURE);
    }
    fp2 = fopen("txt/softtotals.txt", "r");
    if (!fp2) {
        puts("Erro: impossivel ler ficheiro 'softtotals.txt'");
        exit(EXIT_FAILURE);
    }

    Strategy *strategy = (Strategy *) calloc(1, sizeof(Strategy));
    
    strategy->hard = NULL;
    strategy->soft = NULL; 
    
    write_matrix((Move ***) &strategy->hard, fp1);
    write_matrix((Move ***) &strategy->soft, fp2);
 
    return strategy;
}

Move get_decision(Player *player, Card *house_card, Strategy *strategy)
{
    bool ace = false;
    int line = 0, column = 0;
    Stack *aux = player->cards;
    while(aux) {
        if (aux->card->id == 12)
            ace = true;
        aux = aux->next;
    }

    if (house_card->id > 0 && house_card->id < 8)
        column = house_card->id;
    else if (house_card->id >=8 && house_card->id < 12)
        column = 8;
    else if (house_card->id == 12)
        column = 9;

    if (ace) {
        if (player->points > 12 && player->points < 19)
            line = player->points - 13;
        else if (player->points >= 19)
            line = 6;
            
        return strategy->soft[line][column];
    }
    else {
        if (player->points >= 4 && player->points <= 8)
            line = 0;
        else if (player->points > 8 && player->points < 17)
            line = player->points - 8;
        else if (player->points >= 17)
            line = 9;

        return strategy->hard[line][column];
    }   
}

void make_decision(List *players, Player *house, Megadeck megadeck, Strategy *strategy)
{
 	Player *player =  find_active_player(players);
	Card *house_card = house->cards->card;
	Move decision = get_decision(player, house_card, strategy);

	switch (decision) {
		case H:
			hit(players, house, megadeck);
			break;
		case S: 
			stand(players, house, megadeck);
			break;
		case R:
			surrender(players, house, megadeck);
			break;
		case D:
			double_bet(players, house, megadeck);
			break;
		case E:
			double_bet(players, house, megadeck);
			break;
		default:
			break;
	}
}
	
	
}
