#ifndef ID3
#define ID3
#include "data_utils.h"

struct decision_tree_s {
  struct decision_tree_s** children;
  char* children_feature_value; //Tableau des valeurs associées aux enfants du noeud courant
  data_set_t* data;
  int feature_id; //Attribut utilisé par le noeud pour séparer les données.
  char* feature_name; //Chaîne de caractère représentant le nom de la feature associé à feature_id
  bool is_leaf;
  int predicted_class;
  char class_name;
};
typedef struct decision_tree_s decision_tree_t;

void free_tree(decision_tree_t* t);

int most_frequent_class(data_set_t* d);

decision_tree_t* recursive_build_id3_tree(data_set_t* data_set, bool* unused_features, int most_frequent_class_above);

decision_tree_t* build_id3_tree(data_set_t* data_set);

int predict_class(decision_tree_t* id3_tree, data_value_t* input);

double compute_error(decision_tree_t* id3_tree, data_set_t* testing_set);

double* compute_confusion_matrix(decision_tree_t* id3_tree, data_set_t* testing_set);
#endif
