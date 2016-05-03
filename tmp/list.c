/*
 * Proof of concept das listas que vamos usar no projeto.
 * Estas são listas de payload genérico,
 * doubly-linked com dummy head node.
 *
 * A desvantagem de utilizar listas de payload genérico é que
 * obrigamos o utilizador a fazer cast do payload para o que ele
 * precisar, o que pode ser chato, adicionar complexidade ao código
 * e/ou criar bugs difíceis de perceber se nos esquecermos de
 * fazer cast da payload... (ou seja, fazia-se dereference dum
 * void pointer, o que é ilegal) E o utilizador tem se fazer free()
 * da payload manualmente.
 *
 * Mas, por outro lado, o programa torna-se mais modular,
 * mais fácil de compreender e abstrai-se assim toda a parte
 * das listas.
 */
#include <stdlib.h>
#include <stdbool.h>
#include "list.h"

void list_prepend(list *head, void *payload)
{
	list *new_head = (list *) calloc((size_t) 1, sizeof(list));

	new_head->payload = payload;

	new_head->next = head->next;
	new_head->prev = head;
	head->next = new_head;
}

void list_append(list *head, void *payload)
{
	list *aux = head; // dummy head
	while(aux->next)
		aux = aux->next;
	list *tail = aux;

	list *new_tail = (list *) calloc((size_t) 1, sizeof(list));

	new_tail->payload = payload;

	new_tail->next = NULL;
	new_tail->prev = tail;
	tail->next = new_tail;
}

bool list_remove(list *node)
{
	list *to_rm = node;
	node->next->prev = to_rm->prev;
	node->prev->next = to_rm->next;
	free(to_rm);
}
