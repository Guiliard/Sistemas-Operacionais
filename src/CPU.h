#ifndef CPU_H
#define CPU_H

#include <stdio.h>
#include <stdlib.h>

typedef struct core {
    short int registers[16];
}core;

void print_register();

#endif
