#include "disc.h"

void init_disc(disc* memory_disc) {
    memory_disc->matriz = malloc(NUM_DISC * sizeof(unsigned short int*));
    for (int i = 0; i < NUM_DISC; i++) {
        memory_disc->matriz[i] = malloc(NUM_DISC * sizeof(unsigned short int));
    }

    if (memory_disc->matriz == NULL) {
        printf("Error: memory allocation failed in disc\n");
        exit(1);
    }

    for (int i = 0; i < NUM_DISC; i++) {
        for (int j = 0; j < NUM_DISC; j++) {
            memory_disc->matriz[i][j] = 0;
        }
    }
}