#ifndef LIST_H
#define LIST_H

// Doubly-linked list with dummy head nodes
struct List {
	void *payload; // payload genérica
	struct List *next;
	struct List *prev;
};
typedef struct List List;

List *list_follow(List *head, int pos);
void list_insert_pos(List *head, int pos, void *payload);
void list_append(List *head, void *payload);
void *list_remove(List *node);
void *list_remove_pos(List *head, int pos);

#endif
