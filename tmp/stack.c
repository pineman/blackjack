#include <stdlib.h>
#include "stack.h"

void stack_push(stack **sp, void *payload)
{
	stack *old = *sp;
	stack *new = (stack *) calloc((size_t) 1, sizeof(stack));
	new->next = old;
	new->payload = payload;
	*sp = new;
}

void *stack_pop(stack **sp)
{
	if (!*sp)
		// TODO: trying to remove NULL stack
		return NULL;

	stack *pop = *sp;
	void *payload = pop->payload;
	*sp = pop->next;
	free(pop);

	return payload;
}
