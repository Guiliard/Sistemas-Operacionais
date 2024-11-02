#include "reader.h"

char* read_program(char *filename) {
    FILE *arq = fopen(filename, "r"); 
    if (arq == NULL) {
        printf("error opening file\n");
        exit(1);
    }

    char *program = NULL;  
    size_t length = 0;     
    int ch;

    while ((ch = fgetc(arq)) != EOF) {
        program = (char*) realloc(program, length + 2);  
        if (program == NULL) {
            printf("memory allocation failed\n");
            exit(1);
        }
        program[length] = ch;
        length++;
        program[length] = '\0';
    }

    fclose(arq);
    return program;  
}

char* get_line_of_program(char *program, int line_number) {

    char* program_copy = strdup(program); 

    char *line = strtok(program_copy, "\n"); 
    for (int i = 0; i < line_number; i++) {
        if (line != NULL) {
            line = strtok(NULL, "\n");  
        } else {
            free(program_copy);
            return NULL;  
        }
    }

    char* line_copy = strdup(line);
    free(program_copy);
    return line_copy;  
}

unsigned short int count_lines(char *program) {
    unsigned short int count = 0;

    char* program_copy = strdup(program);

    char *line = strtok(program_copy, "\n"); 

    while (line != NULL) {
        count++; 
        line = strtok(NULL, "\n"); 
    }

    return count; 
}

unsigned short int count_tokens_in_line(char *line) {
    unsigned short int count = 0;

    char* line_copy = strdup(line);

    char *token = strtok(line_copy, " "); 

    while (token != NULL) {
        count++; 
        token = strtok(NULL, " "); 
    }

    free(line_copy);
    return count; 
}