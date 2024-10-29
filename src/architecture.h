#ifndef ARCHITECTURE_H
#define ARCHITECTURE_H

#include "cpu.h"
#include "ram.h"
#include "disc.h"
#include "peripherals.h"
#include "cache.h"

void init_architecture(cpu* cpu, ram* memory_ram, disc* memory_disc, peripherals* peripherals);
void load_program_on_ram(ram* memory, char* program);
void check_instructions_on_ram(ram* memory);
void pipiline(cpu* cpu, ram* memory);
char* instruction_fetch(cpu* cpu, ram* memory);
void instruction_decode(char* instruction, unsigned short int num_instruction);

#endif
