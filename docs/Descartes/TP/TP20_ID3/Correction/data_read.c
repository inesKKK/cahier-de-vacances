#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>
#include <stdio.h>
#include "data_read.h"

data_signature_t* read_data_signature(char* signature_filename){
  data_signature_t* signature = (data_signature_t*)malloc(sizeof(data_signature_t));
  FILE* sig_file = fopen(signature_filename, "r");
  //On ignore les commentaires. C'est une boucle while vide et le principal se passe dans l'évaluation de la condition.
  char* comment_content = (char*)malloc(sizeof(char)*1000);
  while(fscanf(sig_file, "//%s\n", comment_content) == 1){
    printf("%s\n", comment_content);
  }
  //On récupère le nombre de features
  int input_size = 0;
  if(fscanf(sig_file, "%d\n", &input_size) == 1){
    signature->input_size = input_size;
    printf("input_size found:%d\n", input_size);
    //On réserve la mémoire pour les noms et le nombre de valeurs une fois le nombre de features connues
    signature->feature_names = (char**)malloc(sizeof(char*) * input_size);
    signature->feature_values_size = (int*)malloc(sizeof(int) * input_size);
    signature->feature_values = (char**)malloc(sizeof(char*) * input_size);

  }
  else{
    assert(false);
  }
  //On lit le nom des différentes features
  for(int i=0; i < input_size; i=i+1){
    //Hypothèse: le nom des features fait moins de 499 caractères. On pourrait mettre ce nombre en argument de la fonction.
    char* current_feature_name = (char*)malloc(sizeof(char) * (499+1));
    int nb_values = 0;
    if(fscanf(sig_file, "%s %d\n", current_feature_name, &nb_values) == 2){
      printf("%d feature found %s:%d\n", i, current_feature_name, nb_values);
      signature->feature_names[i] = current_feature_name;
      signature->feature_values_size[i] = nb_values;
      //On réserve la mémoire pour les différentes valeurs une fois celles ci connues.
      signature->feature_values[i] = (char*)malloc(sizeof(char) * nb_values);
    }

    else{
      assert(false);
    }
    //On récupère maintenant les nb_values différentes valeurs
    for(int j=0; j < nb_values; j=j+1){
      //Hypothèse: la valeur des features est un caractère.
      char current_feature_value;
      if(fscanf(sig_file, "%c\n", &current_feature_value) == 1){
        signature->feature_values[i][j] = current_feature_value;
      }
      else{
        assert(false);
      }
    }
  }
  //On récupère le nombre de classes
  int class_count = 0;
  if(fscanf(sig_file, "%d\n", &class_count) == 1){
    signature->class_count = class_count;
    //On réserve la mémoire pour les noms une fois le nombre de classes connues
    signature->class_names = (char*)(malloc(sizeof(char) * class_count));
  }
  else{
    assert(false);
  }
  for(int i = 0; i<class_count; i = i+1){
    char current_class;
    if(fscanf(sig_file, "%c\n", &current_class) == 1){
      signature->class_names[i] = current_class;
    }
    else{
      assert(false);
    }
  }
  fclose(sig_file);
  free(comment_content);
  return signature;
}

data_set_t* read_data(char* data_filename, data_signature_t* signature){
  data_set_t* data_set = (data_set_t*)malloc(sizeof(data_set_t));
  data_set->signature = signature;
  int data_set_tmp_size = 0;
  FILE* data_file = fopen(data_filename, "r");
  char* line = (char*)malloc(sizeof(char)* 500);
  while(fscanf(data_file, "%s\n", line) != EOF){ //On compte le nombre de lignes du fichier
    data_set_tmp_size = data_set_tmp_size + 1;
  }
  printf("found %d lines\n", data_set_tmp_size);
  fclose(data_file);
  free(line);
  data_file = fopen(data_filename, "r");
  data_value_t** data_array = malloc(sizeof(data_value_t*) * data_set_tmp_size);
  data_set->data_array=data_array;
  int data_set_real_size = 0;
  int input_size = signature-> input_size;
  bool line_read = true;
  for(int i = 0; i < data_set_tmp_size; i = i+1){
    //On lit chacune des lignes
    data_value_t* current_data = (data_value_t*)malloc(sizeof(data_value_t));
    current_data-> id = data_set_real_size;
    current_data->features = (int*)malloc(sizeof(int) * input_size);
    char current_class;
    if(fscanf(data_file, "%c", &current_class) == 1){
      int class_id = -1;
      for(int j = 0; j < signature->class_count; j = j+1){
        if(signature->class_names[j] == current_class){
          class_id = j;
        }
      }
      assert(class_id != -1);
      current_data->class = class_id;
      /* printf("class:%c (%d) ", current_class, class_id); */
    }
    else{
      printf("fail to read class");
      line_read = false;
    }
    //On lit maintenant les features
    for(int j = 0; j < input_size; j = j+1){
      char current_feature_value;
      if(line_read && fscanf(data_file, ",%c", &current_feature_value) == 1){
        int feature_id = -1;
        for(int k = 0; k < signature->feature_values_size[j]; k = k+1){
          if(signature->feature_values[j][k] == current_feature_value){
            feature_id = k;
          }
        }
        /* printf("l_%d x_%d: %c (%d)\n", i, j, current_feature_value, feature_id); */
        assert(feature_id != -1);
        current_data->features[j] = feature_id;
      }
      else{
        printf("failed to read feature");
        line_read = false;
        break;
      }
    }
    /* printf("\n"); */
    if(fscanf(data_file, "\n") == 0){} //Passage à la ligne suivante
    if(line_read){
      data_set->data_array[data_set_real_size] = current_data;
      data_set_real_size = data_set_real_size+1;
    }
  }
  data_set->size = data_set_real_size;
  fclose(data_file);
  return data_set;
}

void free_signature(data_signature_t* sig){
  for(int i = 0; i< sig->input_size; i = i+1){
    free(sig->feature_names[i]);
    free(sig->feature_values[i]);
  }
  free(sig->feature_names);
  free(sig->feature_values);
  free(sig->feature_values_size);
  free(sig->class_names);
  free(sig);
}
void free_data(data_value_t* d){
  free(d->features);
  free(d);
}
void free_data_set(data_set_t* ds){
  for(int i=0; i< ds->size; i = i+1){
    free_data(ds->data_array[i]);
  }
  free(ds->data_array);
  free(ds);
}
