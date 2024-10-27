#ifndef INTERPRETER_H
#define INTERPRETER_H

#include "libs.h"
#include "reader.h"

typedef enum {
    EQUAL,
    NOT_EQUAL,
    LESS_THAN,
    GREATER_THAN,
    LESS_EQUAL,
    GREATER_EQUAL
} signs;

char** get_instruction(char *line, int line_number);
char** check_load_format(char *line);
char** check_store_format(char *line);
char** check_add_format(char *line);
char** check_sub_format(char *line);
char** check_mul_format(char *line);
char** check_div_format(char *line);
char** check_if_format(char *line, signs *sign);
char** check_else_format(char *line);
char** check_loop_format(char *line);

#endif