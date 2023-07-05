#ifndef DATA_UTILS
#define DATA_UTILS
#include "data_read.h"
#include <stdbool.h>

void random_split_data(data_set_t* initial_data_set, float fraction, data_set_t* new_ds_1, data_set_t* new_ds_2);
data_set_t** split_data_set(data_set_t* initial_data_set, int feature_id);
double compute_entropy(data_set_t* d);
double compute_information_gain(data_set_t* d, int feature_id);
int best_information_gain(data_set_t* d, bool* unused_features);
void light_free_data_set(data_set_t* ds);
#endif
