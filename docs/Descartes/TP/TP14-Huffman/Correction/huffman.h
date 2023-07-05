#ifndef HUFFMAN_H
#define HUFFMAN_H
#include "tree.h"
#include "heap.h"
#include "io.h"

heap_t* build_heap_from_text(char* text);
tree_t* build_code_from_heap(heap_t*);
char* encode(char* text, tree_t* huffman_tree);
char* decode(char* text, tree_t* huffman_tree);
void encode_from_file(char* src_filename, char* dst_filename, char* tree_filename);
void decode_from_file(char* src_filename, char* dst_filename, char* tree_filename);
#endif
