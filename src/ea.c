#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LINE_SIZE 12

// hit, stand, surrender, double->hit, double->stand
typedef enum {H='H', S='S', R='R', D='D', E='E'} Moves;

typedef struct Strategy {
    Moves **hard;
    Moves **soft;
} Strategy;

void write_matrix(Moves ***matrix, FILE *fp)
{ 
    char buffer[MAX_LINE_SIZE] = {0};
    for (int i=0; fgets(buffer, MAX_LINE_SIZE, fp); i++) {
        *matrix = (Moves **) realloc(*matrix, (i+1) * sizeof(Moves *));
        (*matrix)[i] = (Moves *) calloc(10, sizeof(Moves));                
         for (int j=0; j<10; j++) {
            (*matrix)[i][j] =  buffer[j];
        }
    }
}

Strategy *read_strategy()
{
    char buffer[MAX_LINE_SIZE] = {0};
    FILE *fp1 = NULL;
    FILE *fp2 = NULL;
    
    fp1 = fopen("hardtotals.txt", "r");
    if (!fp1) {
        puts("Erro: impossivel ler ficheiro 'hardtotals.txt'");
        exit(EXIT_FAILURE);
    }
    fp2 = fopen("softtotals.txt", "r");
    if (!fp2) {
        puts("Erro: impossivel ler ficheiro 'softtotals.txt'");
        exit(EXIT_FAILURE);
    }

    Strategy *strategy = (Strategy *) calloc(1, sizeof(Strategy));
    
    strategy->hard = NULL;d
    strategy->soft = NULL; 
    
    write_matrix((Moves ***) &strategy->hard, fp1);
    write_matrix((Moves ***) &strategy->soft, fp2);
 
    return strategy;
}

void decision(Player *player, Card house_card)
{
    
}
                 
int main()
{
    Strategy *strategy = NULL;
    strategy = read_strategy();

    return EXIT_SUCCESS;
}                   
            
        
