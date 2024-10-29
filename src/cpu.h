#ifndef CPU_H
#define CPU_H

#define NUM_CORES 4
#define NUM_REGISTERS 32

#include "libs.h"
#include "interpreter.h"

typedef struct core {
    unsigned short int *registers;
    unsigned short int PC;
} core;

typedef struct cpu {
   core *core;
} cpu;

typedef enum ula_operation {
    ADD,
    SUB,
    MUL,
    DIV
} ula_operation;

void init_cpu(cpu* cpu);

void control_unit(cpu* cpu, type_of_instruction type, char* instruction);
unsigned short int ula(unsigned short int operating_a, unsigned short int operating_b, ula_operation operation);

unsigned short int get_register_index(char* reg_name);
void load(cpu* cpu, char* instruction);

#endif