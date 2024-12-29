#ifndef READER_H
#define READER_H

#include "libs.h"

char* read_program(char *filename);
char* get_line_of_program(const char *program, int line_number);
unsigned short int count_lines(const char *program,unsigned short int nump);
unsigned short int first_line(const char *program,unsigned short int nump);
unsigned short int count_tokens_in_line(char *line);

#endif