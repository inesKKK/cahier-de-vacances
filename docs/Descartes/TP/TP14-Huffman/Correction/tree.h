#ifndef TREE_H
#define TREE_H

#include <stdbool.h>

struct tree_s
  {
    int weight;
    struct tree_s *left_child;
    struct tree_s *right_child;
    bool is_leaf;
    char leaf_label;
  };
typedef struct tree_s tree_t;

tree_t* init_tree(char leaf_label, int weight);

void free_tree(tree_t* t);

tree_t* merge_trees(tree_t* left_child, tree_t* right_child);

int tree_height(tree_t* t);

int tree_size(tree_t* t);

void print_tree(tree_t* t);

void write_tree(tree_t* t, char* filename);

tree_t* read_tree(char* filemame);
#endif
