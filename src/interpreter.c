#include "interpreter.h"

char** get_instruction(char *line, int line_number) {

    char** instruction = NULL;

    if (strstr(line, "LOAD") != NULL) {
        instruction = check_load_format(line);
        if (instruction != NULL) {
            return instruction;
        } else {
            printf("Error: Invalid LOAD instruction on line %d\n", line_number + 1);
            return NULL;
        }
        
    } else if (strstr(line, "STORE") != NULL) {
        instruction = check_store_format(line);
        if (instruction != NULL) {
            return instruction;
        } else {
            printf("Error: Invalid STORE instruction on line %d\n", line_number + 1);
            return NULL;
        }
    } else if (strstr(line, "ADD") != NULL) {
        instruction = check_add_format(line);
        if (instruction != NULL) {
            return instruction;
        } else {
            printf("Error: Invalid ADD instruction on line %d\n", line_number + 1);
            return NULL;
        }
    } else if (strstr(line, "SUB") != NULL) {
        instruction = check_sub_format(line);
        if (instruction != NULL) {
            return instruction;
        } else {
            printf("Error: Invalid SUB instruction on line %d\n", line_number + 1);
            return NULL;
        }
    } else if (strstr(line, "MUL") != NULL) {
        instruction = check_mul_format(line);
        if (instruction != NULL) {
            return instruction;
        } else {
            printf("Error: Invalid MUL instruction on line %d\n", line_number + 1);
            return NULL;
        }
    } else if (strstr(line, "DIV") != NULL) {
        instruction = check_div_format(line);
        if (instruction != NULL) {
            return instruction;
        } else {
            printf("Error: Invalid DIV instruction on line %d\n", line_number + 1);
            return NULL;
        }
    } else if (strstr(line, "IF") != NULL) {
        signs sign;
        instruction = check_if_format(line, &sign);
        if (instruction != NULL) {
            return instruction;
        } else {
            printf("Error: Invalid IF instruction on line %d\n", line_number + 1);
            return NULL;
        }
    } else if (strstr(line, "ENDIF") != NULL) {
        //return ENDIF;
    } else if (strstr(line, "ELSE") != NULL) {
        //return ELSE;
    } else if (strstr(line, "LOOP") != NULL) {
        //return LOOP;
    } else {
        printf("Error: Unrecognized instruction on line %d\n", line_number);
        return NULL;
    }
}


char** check_load_format(char *line) {

    char **instruction = malloc(3 * sizeof(char*));     

    for (int i = 0; i < 3; i++) {
        instruction[i] = malloc(10 * sizeof(char));  
    }

    unsigned short int value;

    unsigned short int num_tokens = sscanf(line, "LOAD %9s %d", instruction[1], &value);

    if (num_tokens == 2) {
        strcpy(instruction[0], "LOAD"); 
        sprintf(instruction[2], "%d", value); 
        return instruction; 
    } else {
        return NULL; 
    }
}

char** check_store_format(char *line) {
    char **instruction = malloc(3 * sizeof(char*));     

    for (int i = 0; i < 3; i++) {
        instruction[i] = malloc(10 * sizeof(char));  
    }

    unsigned short int num_tokens = sscanf(line, "STORE %9s %9s", instruction[1], instruction[2]);

    if (num_tokens == 2) { 
        strcpy(instruction[0], "STORE"); 
        return instruction; 
    } else {
        return NULL; 
    }
}

char** check_add_format(char *line) {
    char **instruction = malloc(3 * sizeof(char*));     

    for (int i = 0; i < 3; i++) {
        instruction[i] = malloc(10 * sizeof(char));  
    }

    unsigned short int num_tokens;
    unsigned short int value;

    num_tokens = sscanf(line, "ADD %9s %hu", instruction[1], &value);

    if (num_tokens == 2) { 
        strcpy(instruction[0], "ADD"); 
        sprintf(instruction[2], "%hu", value); 
        return instruction; 
    } else {
        num_tokens = sscanf(line, "ADD %9s %9s", instruction[1], instruction[2]);
        if (num_tokens == 2) {
            strcpy(instruction[0], "ADD"); 
            return instruction; 
        } else {
            return NULL;
        }
    }
}

char** check_sub_format(char *line) {
    char **instruction = malloc(3 * sizeof(char*));     

    for (int i = 0; i < 3; i++) {
        instruction[i] = malloc(10 * sizeof(char));  
    }

    unsigned short int num_tokens;
    unsigned short int value;

    num_tokens = sscanf(line, "SUB %9s %hu", instruction[1], &value);

    if (num_tokens == 2) { 
        strcpy(instruction[0], "SUB"); 
        sprintf(instruction[2], "%hu", value); 
        return instruction; 
    } else {
        num_tokens = sscanf(line, "SUB %9s %9s", instruction[1], instruction[2]);
        if (num_tokens == 2) {
            strcpy(instruction[0], "SUB"); 
            return instruction; 
        } else {
            return NULL;
        }
    }
}

char** check_mul_format(char *line) {
    char **instruction = malloc(3 * sizeof(char*));     

    for (int i = 0; i < 3; i++) {
        instruction[i] = malloc(10 * sizeof(char));  
    }

    unsigned short int num_tokens;
    unsigned short int value;

    num_tokens = sscanf(line, "MUL %9s %hu", instruction[1], &value);

    if (num_tokens == 2) { 
        strcpy(instruction[0], "MUL"); 
        sprintf(instruction[2], "%hu", value); 
        return instruction; 
    } else {
        num_tokens = sscanf(line, "MUL %9s %9s", instruction[1], instruction[2]);
        if (num_tokens == 2) {
            strcpy(instruction[0], "MUL"); 
            return instruction; 
        } else {
            return NULL;
        }
    }
}

char** check_div_format(char *line) {
    char **instruction = malloc(3 * sizeof(char*));     

    for (int i = 0; i < 3; i++) {
        instruction[i] = malloc(10 * sizeof(char));  
    }

    unsigned short int num_tokens;
    unsigned short int value;

    num_tokens = sscanf(line, "DIV %9s %hu", instruction[1], &value);

    if (num_tokens == 2) { 
        strcpy(instruction[0], "DIV"); 
        sprintf(instruction[2], "%hu", value); 
        return instruction; 
    } else {
        num_tokens = sscanf(line, "DIV %9s %9s", instruction[1], instruction[2]);
        if (num_tokens == 2) {
            strcpy(instruction[0], "DIV"); 
            return instruction; 
        } else {
            return NULL;
        }
    }
}

char** check_if_format(char *line, signs *sign) {
    char **instruction = malloc(4 * sizeof(char*));  

    for (int i = 0; i < 4; i++) {
        instruction[i] = malloc(10 * sizeof(char));  
    }

    char operator[3];  
    int value;
    unsigned short int num_tokens;

    num_tokens = sscanf(line, "IF %9s %2s %9s", instruction[1], operator, instruction[3]);

    if (num_tokens == 3) {  
        strcpy(instruction[0], "IF");

        if (strcmp(operator, "==") == 0) *sign = EQUAL;
        else if (strcmp(operator, "!=") == 0) *sign = NOT_EQUAL;
        else if (strcmp(operator, "<") == 0) *sign = LESS_THAN;
        else if (strcmp(operator, ">") == 0) *sign = GREATER_THAN;
        else if (strcmp(operator, "<=") == 0) *sign = LESS_EQUAL;
        else if (strcmp(operator, ">=") == 0) *sign = GREATER_EQUAL;
        else return NULL;  

        return instruction;
    } else {
        num_tokens = sscanf(line, "IF %9s %2s %d", instruction[1], operator, &value);

        if (num_tokens == 3) {  // Se o segundo valor é um número
            strcpy(instruction[0], "IF");
            sprintf(instruction[3], "%d", value);

            if (strcmp(operator, "==") == 0) *sign = EQUAL;
            else if (strcmp(operator, "!=") == 0) *sign = NOT_EQUAL;
            else if (strcmp(operator, "<") == 0) *sign = LESS_THAN;
            else if (strcmp(operator, ">") == 0) *sign = GREATER_THAN;
            else if (strcmp(operator, "<=") == 0) *sign = LESS_EQUAL;
            else if (strcmp(operator, ">=") == 0) *sign = GREATER_EQUAL;
            else return NULL;

            return instruction;
        }
    }

    return NULL;
}


