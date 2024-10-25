#include "interpreter.h"

char** get_instruction(char *line, int line_number) {
    if (strstr(line, "LOAD") != NULL) {
        char** instruction = check_load_format(line);
        if (instruction != NULL) {
            return instruction;
        } else {
            printf("Error: Invalid LOAD instruction on line %d\n", line_number + 1);
            return NULL;
        }
        
    } else if (strstr(line, "STORE") != NULL) {
        char** instruction = check_store_format(line);
        if (instruction != NULL) {
            return instruction;
        } else {
            printf("Error: Invalid STORE instruction on line %d\n", line_number + 1);
            return NULL;
        }
    } else if (strstr(line, "ADD") != NULL) {
        //return ADD;
    } else if (strstr(line, "SUB") != NULL) {
        //return SUB;
    } else if (strstr(line, "MUL") != NULL) {
        //return MUL;
    } else if (strstr(line, "DIV") != NULL) {
        //return DIV;
    } else if (strstr(line, "IF") != NULL) {
        //return IF;
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

    unsigned short int num_tokens = sscanf(line, "LOAD %19s %d", instruction[1], &value);

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

    unsigned short int value;

    unsigned short int num_tokens = sscanf(line, "STORE %19s %d", instruction[1], &value);

    if (num_tokens == 2) {
        strcpy(instruction[0], "STORE"); 
        sprintf(instruction[2], "%d", value); 
        return instruction; 
    } else {
        return NULL; 
    }
}