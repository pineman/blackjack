#ifndef LIST_H
#define LIST_H

// Doubly-linked list with dummy head nodes
typedef struct list {
	void *payload; // payload genérica
	struct list *next;
	struct list *prev;
} list;

static list default_dummy_head = {NULL, NULL, NULL};

void list_prepend(list *head, void *payload);
void list_append(list *head, void *payload);
void *list_remove(list *node);

#endif
