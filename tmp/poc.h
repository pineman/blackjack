#ifndef POC_H
#define POC_H

// estes não ficam aqui!!
#define	DECK_SIZE 52
#define SUIT_SIZE 13
#define MAX_CARD_HAND 11

int give_card(list *megadeck, int *cards_left, int num_decks, player *player);
int create_megadeck(list *megadeck, int num_decks);
void destroy_list(list *head);
void destroy_stack(stack **cards);
void print_list(list *megadeck);
void print_reverse_stack(stack *sp);

#endif
