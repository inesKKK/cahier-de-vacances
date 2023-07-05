#include "data_utils.h"
#include "data_read.h"
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>


int main(){
  data_signature_t* sig = read_data_signature("mushroom_signature.data");
  data_set_t* ds = read_data("mushroom.csv", sig);
  data_set_t* new_ds_1 = (data_set_t*)malloc(sizeof(data_set_t));
  data_set_t* new_ds_2 = (data_set_t*)malloc(sizeof(data_set_t));
  random_split_data(ds, 0.9, new_ds_1, new_ds_2);
  h = compute_entropy(new_ds_1);
  printf("%f\n", h);
  h = compute_entropy(new_ds_2);
  printf("%f\n", h);
  double h = compute_entropy(ds);
  printf("%f\n", h);
  free(new_ds_1->data_array);
  free(new_ds_2->data_array);
  free(new_ds_1);
  free(new_ds_2);
  bool* unused_features = (bool*)malloc(sizeof(bool) * ds->signature->input_size);
  for(int i = 0; i < ds->signature->input_size; i = i+1){
    unused_features[i] = true;
  }
  printf("%d\n", best_information_gain(ds, unused_features));
  free(unused_features);
  free_data_set(ds);
  free_signature(sig);
  return 0;
}
