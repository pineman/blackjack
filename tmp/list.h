#ifndef LIST_H
#define LIST_H

// Doubly-linked list with dummy head nodes
struct List {
	void *payload; // payload genérica
	struct List *next;
	struct List *prev;
};
typedef struct List List;

//void list_prepend(List *head, void *payload);
void list_append(List *head, void *payload);
void *list_remove(List *node);

#endif
