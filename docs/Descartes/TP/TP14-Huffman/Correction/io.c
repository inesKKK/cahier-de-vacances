#include <stdio.h>
#include <stdlib.h>
#include <string.h>


int file_len(char* filename){
  FILE* f = fopen(filename, "r");
  int len = 0;
  int nb_values_found = 0;
  char char_content;
  while(nb_values_found != EOF){
    nb_values_found = fscanf(f, "%c", &char_content);
    len = len + 1;
  }
  fclose(f);
  return len;
}

char* read_file_as_string(char* filename){
  int n = file_len(filename);
  char* file_content = (char*)malloc(sizeof(char) * (n+1));
  FILE* f = fopen(filename, "r");
  int nb_values_found = 0;
  int current_pos = 0;
  char char_content;
  while(nb_values_found != EOF){
    nb_values_found = fscanf(f, "%c", &char_content);
    if(nb_values_found == 1){
      file_content[current_pos] = char_content;
      current_pos = current_pos + 1;
    }
  }
  fclose(f);
  file_content[current_pos] = '\0';
  return file_content;
}

void write_content_to_file(char* filename, char* content){
  FILE* f = fopen(filename, "w");
  int n = strlen(content);
  for(int i = 0; i < n; i++){
    fprintf(f, "%c", content[i]);
  }
  fclose(f);
}
