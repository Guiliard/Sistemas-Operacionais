#include "ram.h"

void init_ram(ram* memory_ram) {
    memory_ram->vector = malloc(NUM_MEMORY * sizeof(char));

    if (memory_ram->vector == NULL) {
            printf("memory allocation failed in ram\n");
            exit(1);
        }
    
    for (unsigned short int i = 0; i < NUM_MEMORY; i++) {
        memory_ram->vector[i] = '\0'; 
    }
}

void print_ram(ram* memory_ram) {
    for (unsigned short int i = 0; i < NUM_MEMORY; i++) {
        if(memory_ram->vector[i] == '\0') {
            printf("_");
        } else {
            printf("%c", memory_ram->vector[i]);
        }
    }
}