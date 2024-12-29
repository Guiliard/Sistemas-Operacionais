#ifndef RAM_H
#define RAM_H

#define NUM_MEMORY 4096

#include "libs.h"

typedef struct ram {
    char *vector;
} ram;

void init_ram(ram* memory);
void print_ram(ram* memory);

#endif 