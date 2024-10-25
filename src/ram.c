#include "ram.h"

void init_ram(ram* memory_ram) {
    memory_ram->vector = malloc(NUM_MEMORY * sizeof(unsigned short int));

    if (memory_ram->vector == NULL) {
            printf("memory allocation failed in ram\n");
            exit(1);
        }
}