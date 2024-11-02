#ifndef PIPELINE_H
#define PIPELINE_H

#include "cpu.h"
#include "ram.h"

char* instruction_fetch(cpu* cpu, ram* memory);
type_of_instruction instruction_decode(char* instruction, unsigned short int num_instruction);
//unsigned short int execute(cpu* cpu, type_of_instruction type, char* instruction);
void execute(cpu* cpu, pipe *p);
void memory_access(cpu* cpu, ram* memory_ram, type_of_instruction type, char* instruction);
void write_back(cpu* cpu, type_of_instruction type, char* instruction, unsigned short int result);

#endif
