#include "tree.h"
#include "heap.h"
#include "huffman.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(){
  tree_t* r1 = init_tree('a', 1);
  tree_t* r2 = init_tree('b', 5);
  tree_t* r = merge_trees(r1, r2);
  /* printf("%d\n", r->weight); */
  heap_t*  heap = init_heap();
  insert_heap(r, heap);
  insert_heap(r1, heap);
  insert_heap(r2, heap);
  while(heap->size > 0){
    tree_t* min_t = extract_heap(heap);
    print_tree(min_t);
  }
  free_heap(heap);
  free_tree(r);
  heap_t* huffman_heap = build_heap_from_text("This sentence is way too short for the Huffman compression scheme to be useful (maybe).");
  tree_t* huffman_tree = build_code_from_heap(huffman_heap);
  print_tree(huffman_tree);
  free_heap(huffman_heap);
  free_tree(huffman_tree);
  //
  char* long_text = "Lorem ipsum dolor sit amet, consectetur adipiscing elit. Sed non risus. Suspendisse lectus tortor, dignissim sit amet, adipiscing nec, ultricies sed, dolor. Cras elementum ultrices diam. Maecenas ligula massa, varius a, semper congue, euismod non, mi. Proin porttitor, orci nec nonummy molestie, enim est eleifend mi, non fermentum diam nisl sit amet erat. Duis semper. Duis arcu massa, scelerisque vitae, consequat in, pretium a, enim. Pellentesque congue. Ut in risus volutpat libero pharetra tempor. Cras vestibulum bibendum augue. Praesent egestas leo in pede. Praesent blandit odio eu enim. Pellentesque sed dui ut augue blandit sodales. Vestibulum ante ipsum primis in faucibus orci luctus et ultrices posuere cubilia Curae; Aliquam nibh. Mauris ac mauris sed pede pellentesque fermentum. Maecenas adipiscing ante non diam sodales hendrerit. Ut velit mauris, egestas sed, gravida nec, ornare ut, mi. Aenean ut orci vel massa suscipit pulvinar. Nulla sollicitudin. Fusce varius, ligula non tempus aliquam, nunc turpis ullamcorper nibh, in tempus sapien eros vitae ligula. Pellentesque rhoncus nunc et augue. Integer id felis. Curabitur aliquet pellentesque diam. Integer quis metus vitae elit lobortis egestas. Lorem ipsum dolor sit amet, consectetuer adipiscing elit. Morbi vel erat non mauris convallis vehicula. Nulla et sapien. Integer tortor tellus, aliquam faucibus, convallis id, congue eu, quam. Mauris ullamcorper felis vitae erat. Proin feugiat, augue non elementum posuere, metus purus iaculis lectus, et tristique ligula justo vitae magna. Aliquam convallis sollicitudin purus. Praesent aliquam, enim at fermentum mollis, ligula massa adipiscing nisl, ac euismod nibh nisl eu lectus. Fusce vulputate sem at sapien. Vivamus leo. Aliquam euismod libero eu enim. Nulla nec felis sed leo placerat imperdiet. Aenean suscipit nulla in justo. Suspendisse cursus rutrum augue. Nulla tincidunt tincidunt mi. Curabitur iaculis, lorem vel rhoncus faucibus, felis magna fermentum augue, et ultricies lacus lorem varius purus. Curabitur eu amet.";
  huffman_heap = build_heap_from_text(long_text);
  huffman_tree = build_code_from_heap(huffman_heap);
  print_tree(huffman_tree);
  char* compressed_text = encode(long_text, huffman_tree);
  free_heap(huffman_heap);
  printf("%s\n", compressed_text);
  char* decompressed_text = decode(compressed_text, huffman_tree);
  printf("%s\n", decompressed_text);
  printf("%d\n", strcmp(decompressed_text, long_text));
  write_tree(huffman_tree, "test_tree.txt");
  free_tree(huffman_tree);
  huffman_tree = read_tree("test_tree.txt");
  print_tree(huffman_tree);
  free(decompressed_text);
  decompressed_text = decode(compressed_text, huffman_tree);
  printf("%s\n", decompressed_text);
  printf("%d\n", strcmp(decompressed_text, long_text));
  free(decompressed_text);
  free_tree(huffman_tree);
  free(compressed_text);
  encode_from_file("lorem_ipsum.txt", "cmp_lorem_ipsum.txt", "lorem_ipsum_tree.txt");
  decode_from_file("cmp_lorem_ipsum.txt", "dcmp_lorem_ipsum.txt", "lorem_ipsum_tree.txt");
  encode_from_file("long_text.txt", "cmp_long_text.txt", "long_text_tree.txt");
  decode_from_file("cmp_long_text.txt", "dcmp_long_text.txt", "long_text_tree.txt");
  return 0;
}
