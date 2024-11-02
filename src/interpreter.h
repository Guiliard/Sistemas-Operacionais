#ifndef INTERPRETER_H
#define INTERPRETER_H

#include "libs.h"
#include "reader.h"

typedef enum type_of_instruction {
    LOAD,
    STORE,
    ADD,
    SUB,
    MUL,
    DIV,
    IF,
    ELSE,
    LOOP,
    L_END,
    I_END,
    ELS_END,
    INVALID,
} type_of_instruction;

type_of_instruction verify_instruction(char *line, unsigned short int line_number);
bool check_load_format(char *line);
bool check_store_format(char *line);
bool check_add_format(char *line);
bool check_sub_format(char *line);
bool check_mul_format(char *line);
bool check_div_format(char *line);
bool check_if_format(char *line);
bool check_else_format(char *line);
bool check_loop_format(char *line);
bool check_loop_end_format(char *line);
bool check_if_end_format(char *line);
bool check_else_end_format(char *line);


#endif