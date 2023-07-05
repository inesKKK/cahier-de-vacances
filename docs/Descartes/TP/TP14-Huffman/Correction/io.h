#ifndef IO_H
#define IO_H
int file_len(char* filename);
char* read_file_as_string(char* filename);
void write_content_to_file(char* filename, char* content);
#endif
