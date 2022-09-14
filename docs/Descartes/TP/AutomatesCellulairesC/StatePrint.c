#include <stdio.h>

FILE* init_picture(char *name, int grid_size, int nb_steps){
  FILE* f = fopen(name, "w");
  fprintf(f, "P1\n %d %d\n", grid_size, nb_steps);
  return f;
}

void print_state_to_file(FILE *f, int grid_state[], int grid_size){
  for (int i=0; i<grid_size; i=i+1){
    fprintf(f, "%d\n", grid_state[i]);
  }
}

void save_picture(FILE *f){
  fclose(f);
}
