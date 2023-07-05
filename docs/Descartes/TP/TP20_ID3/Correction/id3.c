#include "id3.h"

#include <stdbool.h>
#include <stdlib.h>
#include <assert.h>
#include <stdio.h>

void free_tree(decision_tree_t* t){
  if(!t->is_leaf){
    int current_feature = t->feature_id;
    int number_of_children = t->data->signature->feature_values_size[current_feature];
    for(int i = 0; i < number_of_children; i = i+1){
      //Le nombre d'enfants est le nombre de valeur pour la feature de la racine.
      free_tree(t->children[i]);
    }
    free(t->children);
    free(t->children_feature_value);
  }
  light_free_data_set(t->data);
  free(t);
}

int most_frequent_class(data_set_t* d){ //compte la classe la plus fréquente d'un data_set. Renvoie -1 si le data_set est vide.
  int* how_many_of_each_class = (int*)malloc(sizeof(int) * d->signature->class_count);
  for(int i = 0; i < d->signature->class_count; i = i+1){
    how_many_of_each_class[i] = 0;
  }
  for(int j = 0; j < d->size; j = j+1){
    int current_class = d->data_array[j]->class;
    how_many_of_each_class[current_class] = how_many_of_each_class[current_class] + 1;
  }
  int most_frequent_class_count = 0;
  int most_frequent_class = 0;
  for(int i = 0; i < d->signature->class_count; i = i+1){
    if(how_many_of_each_class[i] > most_frequent_class_count){
      most_frequent_class_count = how_many_of_each_class[i];
      most_frequent_class = i;
    }
  }
  free(how_many_of_each_class);
  if(most_frequent_class_count == 0){
    return -1;
  }
  else{
    return most_frequent_class;
  }
}

decision_tree_t* recursive_build_id3_tree(data_set_t* data_set, bool* unused_features, int most_frequent_class_above){
  decision_tree_t* current_node = (decision_tree_t*)malloc(sizeof(decision_tree_t));
  current_node->data = data_set;
  int best_feature = best_information_gain(data_set, unused_features);
  current_node->feature_id = best_feature;
  int current_most_frequent_class = most_frequent_class(data_set);
  if (best_feature != -1){
    unused_features[best_feature] = false;
    current_node->is_leaf = false;
    current_node->predicted_class = -1;
    current_node->class_name = '\n';
    int number_of_children = data_set->signature->feature_values_size[best_feature];
    data_set_t** data_for_children = split_data_set(data_set, best_feature);
    decision_tree_t** children = (decision_tree_t**)malloc(sizeof(decision_tree_t*) * number_of_children);
    current_node->children = children;
    current_node->children_feature_value = malloc(sizeof(char) * number_of_children);
    current_node->feature_name = data_set->signature->feature_names[best_feature]; //À ne pas libérer
    for(int i = 0; i< number_of_children; i = i+1){
      current_node->children_feature_value[i] = data_set->signature->feature_values[best_feature][i];
      current_node->children[i] = recursive_build_id3_tree(data_for_children[i], unused_features, current_most_frequent_class);
    }
    free(data_for_children);
    unused_features[best_feature] = true; //Une fois les sous arbres construits, il faut remettre unused_features comme on l'a trouvé pour que la construction des arbres frères se fasse correctement.
  }
  else{
    current_node->is_leaf = true;
    current_node->children = NULL;
    if (data_set->size == 0){
      current_node->predicted_class = most_frequent_class_above;
    }
    else{
      current_node->predicted_class = current_most_frequent_class;
    }
    current_node->class_name = data_set->signature->class_names[current_node->predicted_class];
  }
  return current_node;
}

decision_tree_t* build_id3_tree(data_set_t* data_set){
  assert(data_set->size > 0);
  bool* unused_features = (bool*)malloc(sizeof(bool) * data_set->signature->input_size);
  for(int i = 0; i < data_set->signature->input_size; i = i+1){
    unused_features[i] = true;
  }
  int current_most_frequent_class = most_frequent_class(data_set);
  decision_tree_t* res = recursive_build_id3_tree(data_set, unused_features, current_most_frequent_class);
  free(unused_features);
  return res;
}

int predict_class(decision_tree_t* id3_tree, data_value_t* input){
  if(id3_tree->is_leaf){
    return id3_tree->predicted_class;
  }
  else{
    int current_feature = id3_tree->feature_id;
    return predict_class(id3_tree->children[input->features[current_feature]], input);
  }
}

double compute_error(decision_tree_t* id3_tree, data_set_t* testing_set){
  double error = 0;
  for(int i = 0; i < testing_set->size; i = i+1){
    if (predict_class(id3_tree, testing_set->data_array[i]) != testing_set->data_array[i]->class){
      error = error + 1;
    }
  }
  return error/(double)(testing_set->size);
}
double* compute_confusion_matrix(decision_tree_t* id3_tree, data_set_t* testing_set){
  int number_of_classes = testing_set->signature->class_count;
  double* confusion_matrix = (double*)malloc(sizeof(double) * number_of_classes * number_of_classes);
  for(int k = 0; k < number_of_classes * number_of_classes; k = k+1){
    confusion_matrix[k] = 0;
  }
  for(int i = 0; i < testing_set->size; i = i+1){
    int true_class = testing_set->data_array[i]->class;
    int predicted_class = predict_class(id3_tree, testing_set->data_array[i]);
    int corresponding_index = predicted_class + true_class  * number_of_classes; //La case i,j de la matrice est la i + nj ième case de la matrice.
    confusion_matrix[corresponding_index] = confusion_matrix[corresponding_index] + 1.0/(double)testing_set->size;
  }
  return confusion_matrix;
}
