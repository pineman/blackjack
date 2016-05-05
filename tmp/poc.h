#ifndef POC_H
#define POC_H

// estes não ficam aqui!!
#define	DECK_SIZE 52
#define SUIT_SIZE 13
#define MAX_CARD_HAND 11

int give_card(List *megadeck, int *cards_left, int num_decks, Player *player);
int create_megadeck(List *megadeck, int num_decks);
void destroy_list(List *head);
void destroy_stack(Stack **cards);
void print_list(List *megadeck);
void print_reverse_stack(Stack *sp);

#endif
