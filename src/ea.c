#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// hit, stand, surrender, double->hit, double->stand
typedef enum {H, S, R, D, E} Moves;

typedef struct Strategy {
    Moves **hardtotals;
    Moves **softtotals;
} Strategy;

Strategy *read_strategy()
{
    FILE fp1 = NULL;
    FILE fp2 = NULL;

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

    Strategy *strategy = (Strategy *) malloc(sizeof(Strategy));
    Moves **(strategy->hardtotals) = (Moves **) malloc(10*sizeof(Moves *);
    for (int i=0; i<10; i++) 
        Moves *(strategy->(hardtotals[i])) = (Moves *) malloc(10*sizeof(Moves);
    
    Moves **(strategy->softtotals) = (Moves **) malloc(7*sizeof(Moves *);
    for (int i=0; i<7; i++) 
        Moves *(strategy->(softtotals[i])) = (Moves *) malloc(10*sizeof(Moves);
        
    for (int i=0; i<10; i++) {
        for (int j=0; (c = getc(fp1)) != '\n'; j++) {
            switch (c) {
                case 'H': 
                    strategy->hardtotals[i][j] = H; 
                    break;
                case 'S':
                    strategy->hardtotals[i][j] = S;
                    break;
                case 'R':
                    strategy->hardtotals[i][j] = R;
                    break;
                case 'E':
                    strategy->hardtotals[i][j] = E;
                    break;
            }
        }
    }

    for (int i=0; i<7; i++) {
        for (int j=0; (c = getc(fp2)) != '\n'; j++) {
            switch (c) {
                case 'H': 
                    strategy->softtotals[i][j] = H; 
                    break;
                case 'S':
                    strategy->softtotals[i][j] = S;
                    break;
                case 'R':
                    strategy->softtotals[i][j] = R;
                    break;
                case 'E':
                    strategy->softtotals[i][j] = E;
                    break;
            }
        }
    }
 
    return *strategy;
}
                    
                    
            
        
