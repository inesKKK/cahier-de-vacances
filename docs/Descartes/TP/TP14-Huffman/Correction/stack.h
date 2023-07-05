#ifndef STACK_H
#define STACK_H
#include <stdbool.h>

struct stack_s {tree_t** data; int capacity; int size;};
typedef struct stack_s stack_t;

stack_t* init_stack();

void free_stack(stack_t* stack);

void push_stack(tree_t* value, stack_t* stack);

tree_t* pop_stack(stack_t* stack);

bool is_empty(stack_t* stack);
#endif
