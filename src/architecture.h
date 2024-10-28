#ifndef ARCHITECTURE_H
#define ARCHITECTURE_H

#include "cpu.h"
#include "ram.h"
#include "disc.h"
#include "peripherals.h"
#include "cache.h"
#include "interpreter.h"

void init_architecture(cpu* cpu, ram* memory_ram, disc* memory_disc, peripherals* peripherals);
void load_program_on_ram(ram* memory, char* program);
void check_instructions_on_ram(ram* memory);

#endif
