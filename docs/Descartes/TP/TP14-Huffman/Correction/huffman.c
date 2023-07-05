#include "huffman.h"

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>


heap_t* build_heap_from_text(char* text){
  int n = strlen(text);
  //Counting char occurrences
  int char_occurences[256] = {0}; //size is known and independent of input
  for(int i = 0; i < n; i++){
    char_occurences[(int)text[i]]++; //counting each char in text
  }
  //building heap
  heap_t* result = init_heap();
  for(int i =0; i < 256; i++){
    if(char_occurences[i] > 0){
        tree_t* tree_from_char = init_tree((char)i, char_occurences[i]);
        insert_heap(tree_from_char, result);
      }
  }
  return result;
}

tree_t* build_code_from_heap(heap_t* heap){
  while(heap->size >1){
    tree_t* t1 = extract_heap(heap);
    tree_t* t2 = extract_heap(heap);
    tree_t* merged_tree = merge_trees(t1, t2);
    insert_heap(merged_tree, heap);
  }
  return extract_heap(heap);
}

void fill_dict_from_tree(char** dict, char* current_code, tree_t* tree){
  if(tree != NULL){
    if(tree->is_leaf){
      strcpy(dict[(int)tree->leaf_label], current_code);
    }
    else{
      int n = strlen(current_code);
      current_code[n] = '0';
      current_code[n+1] = '\0';
      fill_dict_from_tree(dict, current_code, tree->left_child);
      current_code[n] = '1';
      current_code[n+1] = '\0';
      fill_dict_from_tree(dict, current_code, tree->right_child);
    }
  }
}

char** code_from_tree(tree_t* huffman_tree){
  //Build a char to code dict
  //First step: get tree height to bound code length
  int h = tree_height(huffman_tree);
  char** code_dict = (char**)malloc(sizeof(char*) * 256); //Array containing the code for each possible char.
  for(int i = 0; i < 256; i++){
    code_dict[i] = (char*)malloc(sizeof(char)*(h+1));
    code_dict[i][0] = '\0'; //Initial codes are empty strings
  }
  char* current_code = (char*)malloc(sizeof(char)*(h+1));
  current_code[0]='\0';
  fill_dict_from_tree(code_dict, current_code, huffman_tree);
  free(current_code);
  return code_dict;
}

void free_code_dict(char** code_dict, int size){
  for(int i = 0; i< size; i++){
    free(code_dict[i]);
  }
  free(code_dict);
}

char* encode(char* text, tree_t* huffman_tree){
  int n = strlen(text);
  int h = tree_height(huffman_tree);
  char* res = (char*)malloc(sizeof(char) * n * h + 1); //compressed_text is smaller than full_text times tree height
  char** code = code_from_tree(huffman_tree);
  int j = 0;
  for(int i = 0; i < n; i++){
    char* code_i = code[(int)text[i]];
    int n_i = strlen(code_i);
    for(int k = 0; k < n_i; k++){
      res[j] = code_i[k];
      j = j+1;
    }
  }
  res[j] = '\0';
  free_code_dict(code, 256);
  return res;
}

int read_next_char(tree_t* huffman_tree, char* read_char, int starting_pos, char* text, int text_len){
  tree_t* current_node = huffman_tree;
  if(current_node->is_leaf){
    *read_char = current_node->leaf_label;
    return starting_pos;
  }
  else if(starting_pos >= text_len){
    *read_char = '\0';
    return starting_pos;
  }
  else if(text[starting_pos] == '0'){
    return read_next_char(huffman_tree->left_child, read_char, starting_pos+1, text, text_len);
  }
  else if(text[starting_pos] == '1'){
    return read_next_char(huffman_tree->right_child, read_char, starting_pos+1, text, text_len);
  }
  else{
    printf("unknown char found %c at pos %d\n", text[starting_pos], starting_pos);
    assert(false);
    return starting_pos;
  }
}

char* decode(char* text, tree_t* huffman_tree){
  int h = tree_height(huffman_tree);
  int n = strlen(text);
  char* res = (char*)malloc(sizeof(char)*(n*h+1));
  int res_pos = 0;
  int text_pos = 0;
  char current_char = '0';
  while(current_char!='\0'){
    text_pos = read_next_char(huffman_tree, &current_char, text_pos, text, n);
    res[res_pos] = current_char;
    res_pos = res_pos + 1;
  }
  res[res_pos] = '\0';
  return res;
}

void encode_from_file(char* src_filename, char* dst_filename, char* tree_filename){
  char* file_content = read_file_as_string(src_filename);
  heap_t* huffman_heap = build_heap_from_text(file_content);
  tree_t* huffman_tree = build_code_from_heap(huffman_heap);
  char* compressed_text = encode(file_content, huffman_tree);
  write_content_to_file(dst_filename, compressed_text);
  free_heap(huffman_heap);
  free(file_content);
  free(compressed_text);
  write_tree(huffman_tree, tree_filename);
  free_tree(huffman_tree);
}

void decode_from_file(char* src_filename, char* dst_filename, char* tree_filename){
  char* file_content = read_file_as_string(src_filename);
  tree_t* huffman_tree = read_tree(tree_filename);
  char* decompressed_text = decode(file_content, huffman_tree);
  write_content_to_file(dst_filename, decompressed_text);
  free(file_content);
  free(decompressed_text);
  free_tree(huffman_tree);
}
