#ifndef INTERPRETER_H
#define INTERPRETER_H

#include "libs.h"
#include "reader.h"

char** get_instruction(char *line, int line_number);
char** check_load_format(char *line);

#endif