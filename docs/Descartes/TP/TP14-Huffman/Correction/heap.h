#ifndef HEAP_H
#define HEAP_H

#include "tree.h"
struct heap_s {tree_t** data; int capacity; int size;}; //heap is built on a resizeable array.
typedef struct heap_s heap_t;

heap_t* init_heap(); //init empty heap

void free_heap(heap_t* heap); //free memory reserved by heap

void insert_heap(tree_t* value, heap_t* heap); //insert value into heap. Priority is value->weight.

tree_t* extract_heap(heap_t* heap); //extract tree with smaller weight from heap.

int heap_size(heap_t* heap);
#endif
