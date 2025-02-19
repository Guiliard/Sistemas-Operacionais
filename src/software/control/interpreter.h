#ifndef INTERPRETER_H
#define INTERPRETER_H

#include "../../utils/libs.h"
#include "../../utils/reader.h"

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
    DELIMITER
} type_of_instruction;

typedef struct similarity_score {
    unsigned short int score_arithmetic;
    unsigned short int score_control;
    unsigned short int score_memory;
} similarity_score;

type_of_instruction verify_instruction(char *line, unsigned short int line_number);
type_of_instruction get_type_of_instruction(char *line);

similarity_score* init_similarity_score();
void get_similarity_score(similarity_score* score, char* line);

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
bool check_delimiter_program_format(char *line);

void trim(char* str);

#endif