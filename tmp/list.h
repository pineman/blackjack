#ifndef LIST_H
#define LIST_H

#include <stdbool.h>

// Doubly-linked list with dummy head nodes
typedef struct list {
	void *payload; // payload genérica
	struct list *next;
	struct list *prev;
} list;

void list_prepend(list *head, void *payload);
void list_append(list *head, void *payload);
bool list_remove(list *head);

#endif
