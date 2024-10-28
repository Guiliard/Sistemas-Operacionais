#include "interpreter.h"

void verify_instruction(char *line, unsigned short int line_number) {

    if (strstr(line, "LOAD") != NULL) {
        if (!check_load_format(line)) {
            printf("Error: Invalid LOAD instruction on line %d\n", line_number + 1);
        }
        
    } else if (strstr(line, "STORE") != NULL) {
        if (!check_store_format(line)) {
            printf("Error: Invalid STORE instruction on line %d\n", line_number + 1);
        }
    } else if (strstr(line, "ADD") != NULL) {
        if (!check_add_format(line)) {
            printf("Error: Invalid ADD instruction on line %d\n", line_number + 1);
        }
    } else if (strstr(line, "SUB") != NULL) {
        if (!check_sub_format(line)) {
            printf("Error: Invalid SUB instruction on line %d\n", line_number + 1);
        }
    } else if (strstr(line, "MUL") != NULL) {
        if (!check_mul_format(line)) {
            printf("Error: Invalid MUL instruction on line %d\n", line_number + 1);
        }
    } else if (strstr(line, "DIV") != NULL) {
        if (!check_div_format(line)) {
            printf("Error: Invalid DIV instruction on line %d\n", line_number + 1);
        }
    } else if (strstr(line, "IF") != NULL) {
        if (!check_if_format(line)) {
            printf("Error: Invalid IF instruction on line %d\n", line_number + 1);
        }
    } else if (strstr(line, "ELSE") != NULL) {
        if (!check_else_format(line)) {
            printf("Error: Invalid ELSE instruction on line %d\n", line_number + 1);
        }
    } else if (strstr(line, "LOOP") != NULL) {
        if (!check_loop_format(line)) {
            printf("Error: Invalid LOOP instruction on line %d\n", line_number + 1);
        }
    } else {
        printf("Error: Unrecognized instruction on line %d\n", line_number);
    } 
}


bool check_load_format(char *line) {  // "LOAD <register> <value>

    char register_name[10];
    unsigned short int value;

    int num_tokens = sscanf(line, "LOAD %9s %hu", register_name, &value);

    unsigned short int num_total_tokens = count_tokens_in_line(line);

    if (num_tokens == 2 && num_total_tokens == 3) {
        return true;
    } else {
        return false;
    }
}

bool check_store_format(char *line) {  // STORE <register> <register>
    char register_name1[10];
    char register_name2[10];
    
    int num_tokens = sscanf(line, "STORE %9s %9s", register_name1, register_name2);

    unsigned short int num_total_tokens = count_tokens_in_line(line);

    if (num_tokens == 2 && num_total_tokens == 3) {
        return true;
    } else {
        return false;
    }
}

bool check_add_format(char *line) {  // ADD <register> <value> ou ADD <register> <register>
    char register_name1[10];
    char register_name2[10];
    unsigned short int value;

    if (sscanf(line, "ADD %9s %hu", register_name1, &value) == 2 && count_tokens_in_line(line) == 3) {
        return true;
    } 
    else if (sscanf(line, "ADD %9s %9s", register_name1, register_name2) == 2 && count_tokens_in_line(line) == 3) {
        return true;
    } 
    else {
        return false;
    }
}

bool check_sub_format(char *line) {  // SUB <register> <value> ou SUB <register> <register>
    char register_name1[10];
    char register_name2[10];
    unsigned short int value;

    if (sscanf(line, "SUB %9s %hu", register_name1, &value) == 2 && count_tokens_in_line(line) == 3) {
        return true;
    } 
    else if (sscanf(line, "SUB %9s %9s", register_name1, register_name2) == 2 && count_tokens_in_line(line) == 3) {
        return true;
    } 
    else {
        return false;
    }
}

bool check_mul_format(char *line) {  // MUL <register> <value> ou MUL <register> <register>
    char register_name1[10];
    char register_name2[10];
    unsigned short int value;

    if (sscanf(line, "MUL %9s %hu", register_name1, &value) == 2 && count_tokens_in_line(line) == 3) {
        return true;
    } 
    else if (sscanf(line, "MUL %9s %9s", register_name1, register_name2) == 2 && count_tokens_in_line(line) == 3) {
        return true;
    } 
    else {
        return false;
    }
}

bool check_div_format(char *line) {  // DIV <register> <value> ou DIV <register> <register>
    char register_name1[10];
    char register_name2[10];
    unsigned short int value;

    if (sscanf(line, "DIV %9s %hu", register_name1, &value) == 2 && count_tokens_in_line(line) == 3) {
        return true;
    } 
    else if (sscanf(line, "DIV %9s %9s", register_name1, register_name2) == 2 && count_tokens_in_line(line) == 3) {
        return true;
    } 
    else {
        return false;
    }
}

bool check_if_format(char *line) {  // IF <register> <operator> <value> ou IF <register> <operator> <register>
    char register_name1[10], register_name2[10];
    char operator[3]; 
    unsigned short int value;

    if (sscanf(line, "IF %9s %2s %9s", register_name1, operator, register_name2) == 3 && count_tokens_in_line(line) == 4) {
        if (strcmp(operator, "==") == 0 || strcmp(operator, "!=") == 0 ||
            strcmp(operator, "<") == 0 || strcmp(operator, ">") == 0 ||
            strcmp(operator, "<=") == 0 || strcmp(operator, ">=") == 0) {
            return true;  
        } else {
            return false; 
        }
    }
    else if (sscanf(line, "IF %9s %2s %hu", register_name1, operator, &value) == 3 && count_tokens_in_line(line) == 4) {
        if (strcmp(operator, "==") == 0 || strcmp(operator, "!=") == 0 ||
            strcmp(operator, "<") == 0 || strcmp(operator, ">") == 0 ||
            strcmp(operator, "<=") == 0 || strcmp(operator, ">=") == 0) {
            return true;  
        } else {
            return false; 
        }
    }

    return false;  
}

bool check_else_format(char *line) {  // ELSE
    int num_tokens = sscanf(line, "ELSE");

    unsigned short int num_total_tokens = count_tokens_in_line(line);

    if (num_tokens == 0 && num_total_tokens == 1) {
        return true;
    } else {
        return false;
    }
}

bool check_loop_format(char *line) {  // LOOP <value> ou LOOP <register>
    char register_name1[10];
    unsigned short int value;

    if (sscanf(line, "LOOP %hu", &value) == 1 && count_tokens_in_line(line) == 2) {
        return true;
    } 
    else if (sscanf(line, "LOOP %9s", register_name1) == 1 && count_tokens_in_line(line) == 2) {
        return true;
    } 
    else {
        return false;
    }
}





