/*
 * stack com 'dummy tail'
 * push: aux = calloc()
 * aux->next = SP
 * SP = aux
 * pop: aux = SP
 * SP = SP->next
 * free(aux)
 */
#include <stdio.h>
#include <stdlib.h>

typedef struct stack {
	void *payload;
	struct stack *next;
} stack;

enum suits {clubs, diamonds, hearts, spades};

typedef struct card {
	enum suits suit;
	int id; // 0 a 12
} card;

void stack_push(stack **sp, void *payload);
void stack_pop(stack **sp);
void remove_cards(stack *player_cards);

int main()
{
	stack dummy = {NULL, NULL};
	stack *player_cards = &dummy;

	card *cur_card1 = calloc(1, sizeof(card));
	cur_card1->suit = clubs;
	cur_card1->id = 0;
	stack_push(&player_cards, cur_card1);

	card *cur_card2 = calloc(1, sizeof(card));
	cur_card2->suit = clubs;
	cur_card2->id = 1;
	stack_push(&player_cards, cur_card2);

	card *cur_card3 = calloc(1, sizeof(card));
	cur_card3->suit = clubs;
	cur_card3->id = 2;
	stack_push(&player_cards, cur_card3);

	stack *aux = player_cards;
	stack *tmp = NULL;
	while (aux->next) { // enquanto não chegámos ao dummy tail
		tmp = aux;
		aux = tmp->next;
		printf("%d: %d\n", ((card *) tmp->payload)->suit, ((card *) tmp->payload)->id);
	}

	remove_cards(player_cards);
}

void stack_push(stack **sp, void *payload)
{
	stack *new = (stack *) calloc((size_t) 1, sizeof(stack));
	new->next = (*sp);
	*sp = new;
	new->payload = payload;
}

void stack_pop(stack **sp)
{
	*sp = (*sp)->next;
}

void remove_cards(stack *player_cards)
{
	stack *aux = player_cards;
	stack *tmp = NULL;
	while (aux->next) { // enquanto não chegámos ao dummy tail
		tmp = aux;
		aux = tmp->next;
		free(tmp->payload);
		free(tmp);
	}

}
