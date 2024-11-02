#include "disc.h"

void init_disc(disc* memory_disc) {
    memory_disc->matriz = malloc(NUM_MEMORY * sizeof(unsigned short int*));
    for (int i = 0; i < NUM_MEMORY; i++) {
        memory_disc->matriz[i] = malloc(NUM_MEMORY * sizeof(unsigned short int));
    }

    if (memory_disc->matriz == NULL) {
        printf("memory allocation failed in disc\n");
        exit(1);
    }

    for (int i = 0; i < NUM_MEMORY; i++) {
        for (int j = 0; j < NUM_MEMORY; j++) {
            memory_disc->matriz[i][j] = 0;
        }
    }
}