#ifndef CPU_H
#define CPU_H

#define NUM_CORES 4
#define NUM_REGISTERS 32

#include "libs.h"

typedef struct cpu {
   core *core;
} cpu;

typedef struct core {
    unsigned short int *registers;
    unsigned short int PC;
} core;

typedef enum opcode {
    LOAD,
    STORE,
    ADD,
    SUB,
    MUL,
    DIV,
    IF,
    ELSE,
    LOOP
} opcode;

typedef enum ula_operation {
    ADD,
    SUB,
    MUL,
    DIV
} ula_operation;

void init_cpu(cpu* cpu);

void control_unit(opcode command);
unsigned short int ula(unsigned short int operating_a, unsigned short int operating_b, ula_operation operation);

#endif