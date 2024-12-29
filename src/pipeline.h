#ifndef PIPELINE_H
#define PIPELINE_H

#include "cpu.h"
#include "ram.h"
#include "control_unit.h"

char* instruction_fetch(cpu* cpu, ram* memory, unsigned short int nump);
type_of_instruction instruction_decode(char* instruction, unsigned short int num_instruction);
void execute(cpu* cpu, ram* memory_ram, instruction_processor* instr_processor, unsigned short int nump);
void memory_access(cpu* cpu, ram* memory_ram, type_of_instruction type, char* instruction);
void write_back(cpu* cpu, type_of_instruction type, char* instruction, unsigned short int result, unsigned short int nump);

#endif