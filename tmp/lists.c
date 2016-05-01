#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct list {
	void *payload;
	struct list *next;
	struct list *prev;
} list;

list *list_prepend(list **old_head, void *payload, size_t size)
{
	list *new_head = (list *) calloc((size_t) 1, sizeof(list));

	new_head->payload = calloc((size_t) 1, size);
	memcpy(new_head->payload, payload, size);
	*old_head = new_head;

	new_head->next = *old_head;
}

int main()
{
	list *head = NULL;
	int inteiro = 5;
	list_prepend(&head, &inteiro, sizeof(inteiro));

	printf("%d\n", *( (int *) head->payload ));
}
