#include "heap.h"
#include "assert.h"

#include <stdlib.h>
#include <stdio.h>

heap_t* init_heap(){
  heap_t* result = (heap_t*)malloc(sizeof(heap_t));
  result->size = 0;
  int capacity = 7; //2^3-1 -> h = 2
  result->capacity = capacity;
  result->data = malloc(sizeof(tree_t*) * capacity);
  return result;
}

void free_heap(heap_t* heap){
  free(heap->data);
  free(heap);
}

int sift_up(heap_t* heap, int index){
  //Swaps the element at index with its father if needed
  //If no father or nothing to do returns -1
  //Else returns new index (previously index of father)
  int parent_node_index = (index-1)/2;
  if(parent_node_index < 0){
    return -1; //index is already the root
  }
  else{
    tree_t* current_node = heap->data[index];
    tree_t* parent_node = heap->data[parent_node_index];
    if(current_node->weight < parent_node->weight){
      heap->data[index] = parent_node;
      heap->data[parent_node_index] = current_node;
      return parent_node_index;
    }
    else{
      return -1;
    }
  }
}

int sift_down(heap_t* heap, int index){
  //Swaps the element at index with its min_child if needed
  //If no child or nothing to do returns index
  //Else returns new index (previously index of min_child)
  int left_child_index = 2*index+1;
  int right_child_index = 2*index+2;
  if(left_child_index >= heap->size && right_child_index >= heap-> size){
    return index; //index is already a leaf
  }
  else{
    tree_t* current_node = heap->data[index];
    tree_t* left_child = heap->data[left_child_index];
    tree_t* right_child = heap->data[right_child_index];
    int min_index;
    tree_t* min_child;
    if(left_child->weight <= right_child->weight){
      min_index = left_child_index;
      min_child = left_child;
    }
    else{
      min_index = right_child_index;
      min_child = right_child;
    }
    if(current_node->weight > min_child->weight){
      heap->data[index] = min_child;
      heap->data[min_index] = current_node;
      return min_index;
    }
    else{
      return index;
    }
  }
}

void insert_heap(tree_t* value, heap_t* heap){
  heap->data[heap->size] = value;
  int current_node_index = heap->size;
  heap->size = heap->size + 1;
  while(current_node_index > 0){
    current_node_index = sift_up(heap, current_node_index);
  }//expanding the heap if it is full
  if(heap->size == heap->capacity){
    tree_t** new_data = (tree_t**)malloc(sizeof(tree_t*) * heap->capacity * 2);
    for(int i = 0; i < heap->size; i++){
      new_data[i] = heap->data[i];
    }
    free(heap->data);
    heap->data = new_data;
    heap->capacity = heap->capacity * 2;
  }
}

tree_t* extract_heap(heap_t* heap){
  assert(heap->size > 0);
  tree_t* min_tree = heap->data[0];
  heap->data[0] = heap->data[heap->size -1];
  heap->size = heap->size - 1;
  int current_node_index = 0;
  int old_node_index = -1;
  while(current_node_index != old_node_index){
    old_node_index = current_node_index;
    current_node_index = sift_down(heap, current_node_index);
  }
  if(heap->size < heap->capacity / 4){ //shrinking heap capacity when too large
    tree_t** new_data = (tree_t**)malloc(sizeof(tree_t*) * heap->capacity / 2);
    for(int i = 0; i < heap->size; i++){
      new_data[i] = heap->data[i];
        }
    free(heap->data);
    heap->data = new_data;
    heap->capacity = heap->capacity / 2;
  }
  return min_tree;
}

void print_heap(heap_t* heap){ //not in priority order !
  for(int i = 0; i < heap->size; i++){
    printf("(%d,%d) ", i, heap->data[i]->weight);
  }
  printf("\n");
}

int heap_size(heap_t* heap){
  return heap->size;
}
