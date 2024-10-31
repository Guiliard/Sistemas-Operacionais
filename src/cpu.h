#ifndef CPU_H
#define CPU_H

#define NUM_CORES 4
#define NUM_REGISTERS 32

#include "libs.h"
#include "interpreter.h"
#include "ram.h"

typedef struct core {
    unsigned short int *registers;
    unsigned short int PC;
} core;

typedef struct cpu {
   core *core;
} cpu;

void init_cpu(cpu* cpu);

unsigned short int control_unit(cpu* cpu, type_of_instruction type, char* instruction);
unsigned short int ula(unsigned short int operating_a, unsigned short int operating_b, type_of_instruction operation);

unsigned short int get_register_index(char* reg_name);
unsigned short int verify_address(ram* memory_ram, char* address, unsigned short int num_positions);

void load(cpu* cpu, char* instruction);
void store(cpu* cpu, ram* memory_ram, char* instruction);

unsigned short int add(cpu* cpu, char* instruction);
unsigned short int sub(cpu* cpu, char* instruction);
unsigned short int mul(cpu* cpu, char* instruction);
unsigned short int div_c(cpu* cpu, char* instruction);
unsigned short int loop(cpu* cpu, char* instruction);

#endif