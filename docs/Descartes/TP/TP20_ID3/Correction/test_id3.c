#include "id3.h"
#include "data_utils.h"
#include "data_read.h"

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

int main(){
  data_signature_t* sig = read_data_signature("mushroom_signature.data");
  data_set_t* ds = read_data("mushroom.csv", sig);
  data_set_t* ds_train = (data_set_t*)malloc(sizeof(data_set_t));
  data_set_t* ds_test = (data_set_t*)malloc(sizeof(data_set_t));
  random_split_data(ds, 0.5, ds_train, ds_test);
  decision_tree_t* id3_tree = build_id3_tree(ds_train);
  printf("first_feature_used: %s\n", id3_tree->feature_name);
  double e = compute_error(id3_tree, ds_test);
  printf("e: %f (pourcent)\n", e*100);
  double* confusion_matrix = compute_confusion_matrix(id3_tree, ds_test);
  for(int j = 0; j < sig->class_count; j = j+1){
    printf("   classe prédite %c  ", sig->class_names[j]);
  }
  for(int i = 0; i < sig->class_count; i = i+1){
    printf("\nvraie classe: %c  ", sig->class_names[i]);
    for(int j = 0; j < sig->class_count; j = j+1){
      printf("%f; ", confusion_matrix[i+sig->class_count*j]);
    }
  }
  printf("\n");
  free(confusion_matrix);
  free_tree(id3_tree);
  light_free_data_set(ds_test);
  free_data_set(ds);
  free_signature(sig);
  return 0;
}
