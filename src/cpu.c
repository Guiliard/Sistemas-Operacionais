#include "cpu.h"

void init_cpu(cpu* cpu) {
    cpu->core = malloc(NUM_CORES * sizeof(core));
    if (cpu->core == NULL) {
        printf("Error: memory allocation failed in cpu\n");
        exit(1);
    }

    for (unsigned short int i = 0; i < NUM_CORES; i++) {
        cpu->core[i].registers = malloc(NUM_REGISTERS * sizeof(unsigned short int));
        if (cpu->core[i].registers == NULL) {
            printf("Error: memory allocation failed in cpu->core[%d].registers\n", i);
            exit(1);
        }
        cpu->core[i].PC = 0;  
        for (unsigned short int j = 0; j < NUM_REGISTERS; j++) {
            cpu->core[i].registers[j] = 0;
        }
    }
}

void reset_cpu(cpu* cpu) {
    for (unsigned short int i = 0; i < NUM_CORES; i++) {
        cpu->core[i].PC = 0;
        for (unsigned short int j = 0; j < NUM_REGISTERS; j++) {
            cpu->core[i].registers[j] = 0;
        }
    }
}