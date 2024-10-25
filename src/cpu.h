#ifndef CPU_H
#define CPU_H

#define NUM_CORES 4
#define NUM_REGISTERS 32

#include "libs.h"

typedef struct cpu {
    unsigned short int *core;
    unsigned short int *registers;
    unsigned short int PC;
} cpu;

void init_cpu(cpu* cpu);

#endif