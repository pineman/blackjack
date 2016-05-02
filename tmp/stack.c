/*
 * stack com 'dummy tail'
 * push: aux = calloc()
 * aux->next = SP
 * SP = aux
 * pop: aux = SP
 * SP = SP->next
 * free(aux)
 */
#include <stdlib.h>
#include "stack.h"

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
