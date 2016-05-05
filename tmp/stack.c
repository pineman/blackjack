#include <stdlib.h>
#include "stack.h"
#include "structs.h"

void stack_push(Stack **sp, Card *card)
{
	Stack *old = *sp;
	Stack *new = (Stack *) calloc((size_t) 1, sizeof(Stack));
	new->next = old;
	new->card = card;
	*sp = new;
}

Card *stack_pop(Stack **sp)
{
	if (!*sp)
		// TODO: trying to remove NULL stack
		return NULL;

	Stack *pop = *sp;
	Card *card = pop->card;
	*sp = pop->next;
	free(pop);

	return card;
}
