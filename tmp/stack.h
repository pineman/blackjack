#ifndef STACK_H
#define STACK_H

#include "structs.h"

void stack_push(Stack **sp, Card *card);
Card *stack_pop(Stack **sp);

#endif
