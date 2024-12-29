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

char* get_line_of_program(const char *program, int line_number) {
    char *program_copy = malloc(strlen(program) + 1);
    char *rest;
    if (!program_copy) {
        fprintf(stderr, "Erro ao alocar memória\n");
        return 0;
    }
    strcpy(program_copy, program);

    char *line = strtok_r(program_copy, "\n", &rest); 
    for (int i = 0; i < line_number; i++) {
        if (line != NULL) {
            line = strtok_r(NULL, "\n", &rest);  
        } else {
            free(program_copy);
            return NULL;  
        }
    }

    char* line_copy = strdup(line);
    free(program_copy);
    return line_copy;  
}

unsigned short int count_lines(const char *program,unsigned short int nump) {
    unsigned short int count = 0, pcount = 0, cnte = 0;
    char *program_copy = malloc(strlen(program) + 1);
    char *rest;
    if (!program_copy) {
        fprintf(stderr, "Erro ao alocar memória\n");
        return 0;
    }
    strcpy(program_copy, program);
    char *line = strtok_r(program_copy, "\n", &rest); 

    while (line != NULL || cnte < nump) {
        if (line[0] == '&') {
            cnte++;
            pcount = count;
            count = 0;
        } else {
            //printf("%s\n",line);
            count++;
        }
        line = strtok_r(NULL, "\n", &rest);
    }

    free(program_copy);
    printf("--%hd(%hd)--\n",pcount,nump);
    return pcount; 
}

unsigned short int first_line(const char *program,unsigned short int nump) {
    unsigned short int count = 0, pcount = 0, tcount = 0, cnte = 0;
    char *program_copy = malloc(strlen(program) + 1);
    char *rest;
    if (!program_copy) {
        fprintf(stderr, "Erro ao alocar memória\n");
        return 0;
    }
    strcpy(program_copy, program);
    char *line = strtok_r(program_copy, "\n", &rest); 

    while (line != NULL || cnte < nump) {
        if (line[0] == '&') {
            cnte++;
            pcount = tcount;
            tcount += count;
            count = 0;             
        } else {
            count++;
        }
        line = strtok_r(NULL, "\n", &rest);
    }

    free(program_copy);
    pcount += (nump - 1);

    return pcount; 
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