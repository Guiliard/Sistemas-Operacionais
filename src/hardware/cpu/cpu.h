#ifndef CPU_H
#define CPU_H

#include "../../utils/libs.h"

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