#ifndef CPU_H
#define CPU_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "uthash.h"

#define NUM_CORES 4
#define NUM_REGISTERS 32
#define NUM_MEMORY 1024

typedef short int si;

typedef struct CPU {
    si core[NUM_CORES];
    si registers[NUM_REGISTERS];
    si PC;
} CPU;

typedef struct cashe {
    UT_hash_handle hash;
} cashe;

typedef struct RAM {
    si vector[NUM_MEMORY];
} RAM;

typedef struct disc {
    si matriz [NUM_MEMORY][NUM_MEMORY];
} disc;

typedef struct peripherals {
    bool input;
} peripherals;

void control_unit(si opcode);
si ula_operation(CPU cpu, si a, si b, si opration);

#endif