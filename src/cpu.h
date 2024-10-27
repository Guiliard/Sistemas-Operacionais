#ifndef CPU_H
#define CPU_H

#define NUM_CORES 4
#define NUM_REGISTERS 32

#include "libs.h"

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

typedef enum registers_bank{
    A0, B0, C0, D0, E0, F0, G0, H0,
    I0, J0, K0, L0, M0, N0, O0, P0,
    A1, B1, C1, D1, E1, F1, G1, H1, 
    I1, J1, K1, L1, M1, N1, O1, P1
} registers_bank;

void init_cpu(cpu* cpu);

void control_unit(cpu* cpu, char** instruction);
unsigned short int ula(unsigned short int operating_a, unsigned short int operating_b, ula_operation operation);
int get_register_index(const char* reg_name);
void load(cpu* cpu, char** instruction);

#endif