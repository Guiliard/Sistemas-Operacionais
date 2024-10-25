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
    char *line = strtok(program, "\n"); 
    for (int i = 0; i < line_number; i++) {
        if (line != NULL) {
            line = strtok(NULL, "\n");  
        } else {
            return NULL;  
        }
    }
    return line;  
}