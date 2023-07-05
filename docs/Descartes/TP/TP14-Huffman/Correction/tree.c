#include "tree.h"
#include "io.h"
#include "stack.h"

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

tree_t* init_tree(char leaf_label, int weight){
  tree_t* result = (tree_t*)malloc(sizeof(tree_t));
  result->is_leaf = true;
  result->leaf_label = leaf_label;
  result->weight = weight;
  result->left_child = NULL;
  result->right_child = NULL;
  return result;
}

void free_tree(tree_t* t){
  if(t->is_leaf){
    free(t);
  }
  else{
    free_tree(t->left_child);
    free_tree(t->right_child);
    free(t);
  }
}

tree_t* merge_trees(tree_t* left_child, tree_t* right_child){
  tree_t* result = (tree_t*)malloc(sizeof(tree_t));
  result->is_leaf = false;
  result->leaf_label = '\0';
  result->weight = left_child->weight + right_child->weight;
  result->left_child = left_child;
  result->right_child = right_child;
  return result;
}

int tree_height(tree_t* t){
  if(t == NULL){
    return -1;
  }
  else if(t->is_leaf){
    return 0;
  }
  else{
    int h_1 = tree_height(t->left_child);
    int h_2 = tree_height(t->right_child);
    if(h_1 > h_2){
      return h_1 + 1;
    }
    else{
      return h_2 + 1;
    }
  }
}

int tree_size(tree_t* t){
  if(t == NULL){
    return 0;
  }
  else if(t->is_leaf){
    return 1;
  }
  else{
    int s_1 = tree_size(t->left_child);
    int s_2 = tree_size(t->right_child);
    return s_1 + s_2 + 1;
  }
}

void print_rec_tree(tree_t* t){
  if(t->is_leaf){
    printf("(%c,%d,0)", t->leaf_label, t->weight);
  }
  else{
    print_rec_tree(t->left_child);
    print_rec_tree(t->right_child);
    printf("( ,%d,2)", t->weight);

  }
}

void print_tree(tree_t* t){
  print_rec_tree(t);
  printf("\n");
}

void write_rec_tree(tree_t* t, FILE* file_stream){
  if(t->is_leaf){
    fprintf(file_stream, "(%c,%d,0)", t->leaf_label, t->weight);
  }
  else{
    write_rec_tree(t->left_child, file_stream);
    write_rec_tree(t->right_child, file_stream);
    fprintf(file_stream, "( ,%d,2)", t->weight);
  }
}

void write_tree(tree_t* t, char* filename){
  FILE* f = fopen(filename, "w");
  write_rec_tree(t, f);
  fclose(f);
}

tree_t* read_tree(char* filename){
  stack_t* stack = init_stack();
  FILE* f = fopen(filename, "r");
  int nb_values_found = 0;
  int weight = 0;
  int nb_children = 0;
  char current_char = '\0';
  while(nb_values_found != EOF){
    nb_values_found = fscanf(f, "(%c,%d,%d)", &current_char, &weight, &nb_children);
    if(nb_values_found == 3){
      if(nb_children == 0){
        tree_t* t = init_tree(current_char, weight);
        push_stack(t, stack);
      }
      else if(nb_children == 2){
        assert(stack->size >= 2);
        tree_t* t2 = pop_stack(stack);
        tree_t* t1 = pop_stack(stack);
        tree_t* merged_tree = merge_trees(t1, t2);
        assert(weight == merged_tree->weight);
        push_stack(merged_tree, stack);
      }
    }
    else if(nb_values_found != EOF){
      printf("pattern not found");
    }
  }
  fclose(f);
  assert(stack->size==1);
  tree_t* result = pop_stack(stack);
  free_stack(stack);
  return result;
}
