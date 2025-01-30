#ifndef CPU_H
#define CPU_H

#define NUM_CORES 2
#define NUM_REGISTERS 32

#include "libs.h"

typedef struct core {
    unsigned short int *registers;
    unsigned short int PC;
} core;

typedef struct cpu {
   core *core;
} cpu;

void init_cpu(cpu* cpu);
void reset_cpu(cpu* cpu, unsigned short int core_id);

#endif