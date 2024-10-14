#ifndef CPU_H
#define CPU_H

#include <stdio.h>
#include <stdlib.h>
#include "ULA.h"

#define MEMORY_SIZE 1024 

typedef struct Core {
    int registers[32];
    int PC;             
    int IR;
} Core;

typedef struct Memory {
    int data[MEMORY_SIZE];
} Memory;

void init_core(Core *c);
void set_register(Core *c, int reg_num, int value);
int get_register(Core *c, int reg_num);
void print_registers(Core *c);

// Funções de memória
void init_memory(Memory *mem);
void write_memory(Memory *mem, int address, int value);
int read_memory(Memory *mem, int address);

#endif