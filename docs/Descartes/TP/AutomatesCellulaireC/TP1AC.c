#include <stdio.h>
#include "StatePrint.c"
//Array syntax
int smaller_power_int(int base, int k){
  int result = 1;
  if (k < 1){
    return 0;
  }
  while (result * base <= k){
    result = result * base;
  }
  if (k == result){ // k is a power of base
    return 0;
  }
  else{
    return result;
  }
}

void init_int_array(int array[], int array_size){
  for (int i=0; i<array_size; i = i+1){
    array[i] = smaller_power_int(2, i);
  }
}

void print_int_array(int t[], int array_size){
  for (int i=0; i <array_size; i+=1){
    if (t[i] == 0){
      printf(" ");
    }
    else{
      printf("*");
    }
  }
  printf("\n");
}

int int_array_sum(int array[], int array_size){
  int result = 0;
  for (int i=0; i<array_size; i=i+1){
    result = result + array[i];
  }
  return result;
}

int int_array_min(int array[], int array_size){
  int result = 0;
  if (array_size == 0){
    return 0;
  }
  else{
    result = array[0];
    for (int i=0; i<array_size; i=i+1){
      if (result > array[i])
        {
          result = array[i];
        }
    }
  }
  return result;
}

int int_array_max(int array[], int array_size){
  int result = 0;
  if (array_size == 0){
    return 0;
  }
  else{
    result = array[0];
    for (int i=0; i<array_size; i=i+1){
      if (result < array[i])
        {
          result = array[i];
        }
    }
  }
  return result;
}
//With double
int smaller_power_double(double base, double k){
  double result = 1;
  if (k < 1){
    return 0;
  }
  while (result * base <= k){
    result = result * base;
  }
  if (k == result){ // k is a power of base
    return 0;
  }
  else{
    return result;
  }
}

void init_double_array(double array[], int array_size){
  for (int i=0; i<array_size; i = i+1){
    array[i] = smaller_power_double(2, i);
  }
}

void print_double_array(double t[], int array_size){
  for (int i=0; i <array_size; i+=1){
    printf("%f ", t[i]);
  }
  printf("\n");
}

double double_array_sum(double array[], int array_size){
  double result = 0;
  for (int i=0; i<array_size; i=i+1){
    result = result + array[i];
  }
  return result;
}

double double_array_min(double array[], int array_size){
  double result = 0;
  if (array_size == 0){
    return 0;
  }
  else{
    result = array[0];
    for (int i=0; i<array_size; i=i+1){
      if (result > array[i])
        {
          result = array[i];
        }
    }
  }
  return result;
}

double double_array_max(double array[], int array_size){
  double result = 0;
  if (array_size == 0){
    return 0;
  }
  else{
    result = array[0];
    for (int i=0; i<array_size; i=i+1){
      if (result < array[i])
        {
          result = array[i];
        }
    }
  }
  return result;
}
//Cellular Automata
int automata_rule(int current_cell, int left_cell, int right_cell){
  if (left_cell == 0 && current_cell == 0 && right_cell == 0){
    return 0;
  }
  else if (left_cell == 0 && current_cell == 0 && right_cell == 1){
    return 1;
  }
  else if(left_cell == 0 && current_cell == 1 && right_cell == 0){
    return 0;
  }
  else if(left_cell == 0 && current_cell == 1 && right_cell == 1){
    return 0;
  }
  else if(left_cell == 1 && current_cell == 0 && right_cell == 0){
    return 1;
  }
  else if(left_cell == 1 && current_cell == 0 && right_cell == 1){
    return 0;
  }
  else if(left_cell == 1 && current_cell == 1 && right_cell == 0){
    return 0;
  }
  else if(left_cell == 1 && current_cell == 1 && right_cell == 1){
    return 1;
  }
  else{
    printf("Unknown entry");
    return -1;
  }
}

void update_state(int grid_now[], int updated_grid[], int grid_size){
  for (int i=0; i<grid_size; i=i+1){
    if(i==0){
      updated_grid[i] = automata_rule(grid_now[i], 0, grid_now[i+1]);
    }
    else if(i==grid_size -1){
      updated_grid[i] = automata_rule(grid_now[i], grid_now[i-1], 0);
    }
    else{
      updated_grid[i] = automata_rule(grid_now[i], grid_now[i-1], grid_now[i+1]);
    }
  }
}

void update_state_bis(int grid_now[], int updated_grid[], int grid_size){
  for (int i=0; i<grid_size; i=i+1){
    updated_grid[i] = automata_rule(grid_now[i], grid_now[(i-1 + grid_size) % grid_size], grid_now[(i+1) % grid_size]);
  }
}

void run_automata(int init_grid[], int previous_step_grid[], int next_step_grid[], int grid_size, int nb_steps){
  print_int_array(init_grid, grid_size);
  for (int i=0; i<grid_size; i =i+1){
    next_step_grid[i] = init_grid[i];
  }
  for (int i=0; i<nb_steps; i=i+1){
    for (int i=0; i<grid_size; i =i+1){
      previous_step_grid[i] = next_step_grid[i];
    }
    update_state(previous_step_grid, next_step_grid, grid_size);
    print_int_array(next_step_grid, grid_size);
  }
}

//Bonus
int from_base_to_int(int base_decomposition[], int decomposition_size, int base){
  int result = 0;
  int base_power = 1;
  for (int i = 0; i<decomposition_size; i=i+1){
    result = result + base_decomposition[i] * base_power;
    base_power = base_power * base;
  }
  return result;
}

void from_int_to_base(int base_decomposition[], int decomposition_size, int base, int n){
  for (int i = 0; i<decomposition_size; i=i+1){
    base_decomposition[i] = n % base;
    n = n/base;
  }
}

int generic_automata_rule(int current_cell, int left_cell, int right_cell, int rule_number){
  int rule_decomposition[8];
  if (rule_number < 256){ //should always be true
    from_int_to_base(rule_decomposition, 8, 2, rule_number);
  }
  else{
    printf("Incorrect rule number, returning 0\n");
    return 0;
  }
  int state_context[3] = {left_cell, current_cell, right_cell};
  int context_number = from_base_to_int(state_context, 3, 2);
  if (context_number <8){ //should always be true
    return rule_decomposition[context_number];
  }
  else{
    printf("Incorrect context, returning 0\n");
    return 0;
  }
}

void update_state_generic(int grid_now[], int updated_grid[], int grid_size, int rule_number){
  for (int i=0; i<grid_size; i=i+1){
    updated_grid[i] = generic_automata_rule(grid_now[i], grid_now[(i-1 + grid_size) % grid_size], grid_now[(i+1) % grid_size], rule_number);
  }
}

void run_generic_automata(int init_grid[], int previous_step_grid[], int next_step_grid[], int grid_size, int nb_steps, int rule_number){
  print_int_array(init_grid, grid_size);
  for (int i=0; i<grid_size; i =i+1){
    next_step_grid[i] = init_grid[i];
  }
  for (int i=0; i<nb_steps; i=i+1){
    for (int i=0; i<grid_size; i =i+1){
      previous_step_grid[i] = next_step_grid[i];
    }
    update_state_generic(previous_step_grid, next_step_grid, grid_size, rule_number);
    print_int_array(next_step_grid, grid_size);
  }
}

void run_generic_automata_with_picture(int init_grid[], int previous_step_grid[], int next_step_grid[], int grid_size, int nb_steps, int rule_number){
  FILE* f = init_picture("CellarAutomata.pbm", grid_size, nb_steps);
  print_state_to_file(f, init_grid, grid_size);
  for (int i=0; i<grid_size; i =i+1){
    next_step_grid[i] = init_grid[i];
  }
  for (int i=0; i<nb_steps; i=i+1){
    for (int i=0; i<grid_size; i =i+1){
      previous_step_grid[i] = next_step_grid[i];
    }
    update_state_generic(previous_step_grid, next_step_grid, grid_size, rule_number);
    print_state_to_file(f, next_step_grid, grid_size);
  }
  save_picture(f);
}



int main(){
  //Array syntax
  int some_int_array[10];
  for (int i=0; i<10; i = i+1){
    some_int_array[i] = i;
  }
  print_int_array(some_int_array, 10);
  printf("%d\n", int_array_sum(some_int_array, 10));
  init_int_array(some_int_array, 10);
  print_int_array(some_int_array, 10);
  printf("%d\n", int_array_sum(some_int_array, 10));
  printf("%d\n", int_array_min(some_int_array, 10));
  printf("%d\n", int_array_max(some_int_array, 10));

  //With doubles
  double some_double_array[10];
  for (int i=0; i<10; i = i+1){
    some_double_array[i] = i;
  }
  print_double_array(some_double_array, 10);
  printf("%f\n", double_array_sum(some_double_array, 10));
  init_double_array(some_double_array, 10);
  print_double_array(some_double_array, 10);
  printf("%f\n", double_array_sum(some_double_array, 10));
  printf("%f\n", double_array_min(some_double_array, 10));
  printf("%f\n", double_array_max(some_double_array, 10));

  // Cellular Automata
  printf("%d\n", automata_rule(0, 1, 0));
  printf("%d\n", automata_rule(0, 1, 1));

  int init_grid[64] = {0};
  init_grid[30] =1;
  int previous_step_grid[64];
  int next_step_grid[64];
  run_automata(init_grid, previous_step_grid, next_step_grid, 64, 30);

  //Bonus
  run_generic_automata(init_grid, previous_step_grid, next_step_grid, 64, 30, 73);
  int init_grid2[512] = {0};
  init_grid2[255] = 1;
  int previous_step_grid2[512];
  int next_step_grid2[512];
  run_generic_automata_with_picture(init_grid2, previous_step_grid2, next_step_grid2, 512, 1000, 110);

  return 0;
}
