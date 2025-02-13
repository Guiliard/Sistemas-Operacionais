#ifndef RAM_H
#define RAM_H

#define NUM_MEMORY 2048

#include "../../utils/libs.h"

typedef struct ram {
    char *vector;
} ram;

void init_ram(ram* memory_ram);
void print_ram(ram* memory_ram);
void write_ram(ram* memory_ram, unsigned short int address, char* buffer);
void reset_ram(ram* memory_ram);

#endif 