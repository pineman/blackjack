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
 *
 * TODO: repensar se valem a pena void pointers...
 */
#include <stdlib.h>
#include <stdbool.h>
#include "list.h"

void list_append(List *head, void *payload)
{
	List *aux = head;
	while (aux->next)
		aux = aux->next;
	List *tail = aux;

	List *new_tail = (List *) calloc((size_t) 1, sizeof(List));

	new_tail->payload = payload;

	new_tail->next = NULL;
	new_tail->prev = tail;
	tail->next = new_tail;
}

void *list_remove(List *node)
{
	List *to_rm = node;
	void *payload = to_rm->payload;

	if (node->next) {
		node->next->prev = to_rm->prev;
	}
	else {
		// removing tail, it has no next, skip.
	}

	if (node->prev) {
		node->prev->next = to_rm->next;
	}
	else {
		// TODO: removing dummy head!! it has no prev! panic
		return NULL;
	}

	free(to_rm);
	return payload;
}
