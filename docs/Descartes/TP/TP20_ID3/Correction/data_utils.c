#include <stdlib.h>
#include <assert.h>
#include <stdio.h>
#include <stdbool.h>
#include <math.h>

#include "data_read.h"

//Remplit new_ds_1 et new_ds_2 avec des données d'initial_data_set avec une probabilité fraction d'attribuer un point à new_ds_1 et le complémentaire pour new_ds_2.
void random_split_data(data_set_t* initial_data_set, float fraction, data_set_t* new_ds_1, data_set_t* new_ds_2){
  bool* is_in_new_ds_1 = (bool*)malloc(sizeof(bool) * initial_data_set->size);
  int ds_1_size = 0;
  int ds_2_size = 0;

  //On commence par déterminer quel point va dans quel data set pour connaître la taille des tableaux à réserver.
  for(int i=0; i<initial_data_set->size; i = i+1){
    if ((double)rand()/(double)RAND_MAX < fraction){
      is_in_new_ds_1[i] = true;
      ds_1_size = ds_1_size + 1;
    }
    else{
      is_in_new_ds_1[i] = false;
      ds_2_size = ds_2_size + 1;
    }
  }
  assert(ds_1_size+ds_2_size == initial_data_set->size);
  //Une fois la taille connue, on peut réserver la mémoire
  data_value_t** ds_1_array = (data_value_t**)malloc(sizeof(data_value_t*) * ds_1_size);
  data_value_t** ds_2_array = (data_value_t**)malloc(sizeof(data_value_t*) * ds_2_size);
  int current_ds_1_index = 0;
  int current_ds_2_index = 0;
  //puis copier les données (ici simplement les pointeurs, les entrées sont donc partagées entre les deux datasets) en prenant soin de noter dans quelle case les insérer dans le nouveau tableau.
  for(int i=0; i<initial_data_set->size; i = i+1){
    if (is_in_new_ds_1[i]){
      ds_1_array[current_ds_1_index] = initial_data_set->data_array[i];
      current_ds_1_index = current_ds_1_index+1;
    }
    else{
      ds_2_array[current_ds_2_index] = initial_data_set->data_array[i];
      current_ds_2_index = current_ds_2_index+1;
    }
  }
  free(is_in_new_ds_1);
  assert(current_ds_1_index == ds_1_size);
  new_ds_1->data_array = ds_1_array;
  new_ds_1->size = ds_1_size;
  new_ds_1->signature = initial_data_set->signature;
  new_ds_2->data_array = ds_2_array;
  new_ds_2->size = ds_2_size;
  new_ds_2->signature = initial_data_set->signature;
  assert(current_ds_2_index == ds_2_size);
}

//Partitionne un data set selon la valeur des d'une des features donnée via son id
data_set_t** split_data_set(data_set_t* initial_data_set, int feature_id){
  int partition_size = initial_data_set->signature->feature_values_size[feature_id];
  data_set_t** partition = (data_set_t**)malloc(sizeof(data_set_t*) * partition_size); //partition[i] contient les élément dont l'attribut prend la i-ième parmi toutes celles possibles.
  int* current_index_partition = malloc(sizeof(int) * partition_size); //la case i indique l'index où insérer l'élément suivant de la partition i
  int* partition_i_size = malloc(sizeof(int) * partition_size); //la case i indique la taille de partition[i]
  for(int i = 0; i< partition_size; i = i+1){
    current_index_partition[i] = 0;
    partition_i_size[i] = 0;
  }
  //On parcourt tout le tableau pour savoir quelle est la taille de chaque élément de la partition
  for(int j = 0; j < initial_data_set->size; j = j+1){
    int value_id = initial_data_set->data_array[j]->features[feature_id];
    partition_i_size[value_id] = partition_i_size[value_id] + 1;
  }
  for(int i = 0; i< partition_size; i = i+1){
    partition[i] = (data_set_t*)malloc(sizeof(data_set_t));
    partition[i]->signature = initial_data_set->signature;
    if(partition_i_size[i] == 0){
      partition[i]->data_array = NULL;
    }
    else{
      partition[i]->data_array = (data_value_t**)malloc(sizeof(data_value_t*) * partition_i_size[i]);
    }
    partition[i]->size = partition_i_size[i];
  }
  for(int j = 0; j < initial_data_set->size; j = j+1){
    int value_id = initial_data_set->data_array[j]->features[feature_id];
    partition[value_id]->data_array[current_index_partition[value_id]] = initial_data_set->data_array[j];
    current_index_partition[value_id] = current_index_partition[value_id]+1;
  }
  free(partition_i_size);
  free(current_index_partition);
  return partition;
}

double compute_entropy(data_set_t* d){
  if (d->size >0){
    int* class_i_count = malloc(sizeof(int) * d->signature->class_count);
    for(int i = 0; i< d->signature->class_count; i = i+1){
      class_i_count[i] =  0;
    }
    for(int j = 0; j< d->size; j = j+1){
      class_i_count[d->data_array[j]->class] = class_i_count[d->data_array[j]->class] + 1;
    }
    double h = 0;
    for(int i = 0; i< d->signature->class_count; i = i+1){
      double freq_i = (double)class_i_count[i]/(double)d->size;
      /* printf("%d %f %d %d\n", i, freq_i, class_i_count[i], d->size); */
      if(freq_i > 0){ //si freq_i = 0, le produit est considéré comme nul
        h = h - freq_i * log(freq_i);
      }
    }
    free(class_i_count);
    return h;
  }
  else{
    return 0; //Convention pour l'entropie d'un ensemble vide.
  }
}

void light_free_data_set(data_set_t* ds){
  free(ds->data_array);
  free(ds);
}

double compute_information_gain(data_set_t* d, int feature_id){
  double info_gain = compute_entropy(d);
  data_set_t** split = split_data_set(d, feature_id);
  for(int i = 0; i<d->signature->feature_values_size[feature_id]; i = i +1){
    info_gain = info_gain - (double)split[i]->size/(double)d->size * compute_entropy(split[i]);
    light_free_data_set(split[i]);
  }
  free(split);
  return info_gain;
}

int best_information_gain(data_set_t* d, bool* unused_features){
  double best_gain_so_far = 0;
  int best_feature_so_far = -1;
  for(int i = 0; i< d->signature->input_size; i = i+1){
    if(unused_features[i]){
      double gain = compute_information_gain(d, i);
      if(gain >= best_gain_so_far){
        best_gain_so_far = gain;
        best_feature_so_far = i;
      }
    }
  }
  return best_feature_so_far;
}
