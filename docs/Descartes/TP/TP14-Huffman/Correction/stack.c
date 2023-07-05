#include "tree.h"
#include "stack.h"

#include <stdlib.h>


stack_t* init_stack(){
  stack_t* result = (stack_t*)malloc(sizeof(stack_t));
  result->size = 0;
  int capacity = 4; //2^2-1 -> h = 2
  result->capacity = capacity;
  result->data = malloc(sizeof(tree_t*) * capacity);
  return result;
}

void free_stack(stack_t* stack){
  free(stack->data);
  free(stack);
}

void push_stack(tree_t* value, stack_t* stack){
  stack->data[stack->size] = value;
  stack->size = stack->size + 1;
  if(stack->size == stack->capacity){
    tree_t** new_data = (tree_t**)malloc(sizeof(tree_t*) * stack->capacity * 2);
    for(int i = 0; i < stack->size; i++){
      new_data[i] = stack->data[i];
    }
    free(stack->data);
    stack->data = new_data;
    stack->capacity = stack->capacity * 2;

  }
}

bool is_empty(stack_t* stack){
  if(stack->size == 0){
    return true;
  }
  else{
    return false;
  }
}

tree_t* pop_stack(stack_t* stack){
  if(!is_empty(stack)){
    tree_t* result = stack->data[stack->size - 1];
    stack->size = stack->size -1;
    if(stack->size < stack->capacity / 4){ //shrinking stack capacity when too large
      tree_t** new_data = (tree_t**)malloc(sizeof(tree_t*) * stack->capacity / 2);
      for(int i = 0; i < stack->size; i++){
        new_data[i] = stack->data[i];
      }
      free(stack->data);
      stack->data = new_data;
      stack->capacity = stack->capacity / 2;
    }
    return result;
  }
  else{
    return NULL;
  }
}
