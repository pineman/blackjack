#ifndef EA_H
#define EA_H

#define MAX_LINE_SIZE 12

// hit, stand, surrender, double->hit, double->stand
typedef enum {H='H', S='S', R='R', D='D', E='E'} Move;

typedef struct Strategy {
    Move **hard;
    Move **soft;
} Strategy;

void write_matrix(Move ***matrix, FILE *fp);
Strategy *read_strategy();
Move get_decision(Player *player, Card *house_card, Strategy *strategy);
void make_decision(List *players, Player *house, Megadeck megadeck, Strategy *strategy);

#endif
