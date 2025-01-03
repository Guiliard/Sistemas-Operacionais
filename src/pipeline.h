#ifndef PIPELINE_H
#define PIPELINE_H

#include "cpu.h"
#include "ram.h"
#include "control_unit.h"

char* instruction_fetch(cpu* cpu, char* program, unsigned short int index_core);
type_of_instruction instruction_decode(char* instruction, unsigned short int num_instruction);
void execute(cpu* cpu, char* program, instruction_processor* instr_processor, unsigned short int index_core);
void memory_access(cpu* cpu, ram* memory_ram, process_control_block* pcb, type_of_instruction type, char* instruction, unsigned short int index_core);
void write_back(cpu* cpu, type_of_instruction type, char* instruction, unsigned short int result, unsigned short int index_core);

#endif