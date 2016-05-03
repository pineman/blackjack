// estes não ficam aqui!!
#define	DECK_SIZE 52
#define SUIT_SIZE 13

int create_megadeck(list *head, int num_decks);
void destroy_megadeck(list *head);
void destroy_player_cards(player *player);
int give_card(
	list *megadeck, int *cards_left, int num_decks,
	player *player);
