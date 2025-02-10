#ifndef CONTROL_UNIT_H
#define CONTROL_UNIT_H

#include "libs.h"
#include "ram.h"
#include "cpu.h"
#include "interpreter.h"
#include "pcb.h"
#include "cache.h"

void control_unit(cpu* cpu, char* program, instruction_processor* instr_processor, unsigned short int index_core, cache2* cache_table);
unsigned short int ula(unsigned short int operating_a, unsigned short int operating_b, type_of_instruction operation, cache2* cache_table);

unsigned short int get_register_index(char* reg_name);
unsigned short int verify_address(char* address, unsigned short int num_positions);

void load(cpu* cpu, char* instruction, process_control_block* pcb, unsigned short int index_core);
void store(cpu* cpu, ram* memory_ram, process_control_block* pcb, char* instruction, unsigned short int index_core);

unsigned short int add(cpu* cpu, char* instruction, unsigned short int index_core, cache2* cache_table);
unsigned short int sub(cpu* cpu, char* instruction, unsigned short int index_core, cache2* cache_table);
unsigned short int mul(cpu* cpu, char* instruction, unsigned short int index_core, cache2* cache_table);
unsigned short int div_c(cpu* cpu, char* instruction, unsigned short int index_core, cache2* cache_table);

void if_i(cpu* cpu, char* program, instruction_processor* instr_processor, unsigned short int index_core);
void if_end(instruction_processor* instr_processor);
void else_i(cpu* cpu, char* program, instruction_processor* instr_processor, unsigned short int index_core);
void else_end(instruction_processor* instr_processor);
void loop(cpu* cpu, instruction_processor* instr_processor, unsigned short int index_core);
void loop_end(cpu* cpu, instruction_processor* instr_processor, unsigned short int index_core);

void add_register_to_bank(process_control_block *pcb, char *register_name);
void add_result_of_process_to_pcb(process_control_block *pcb, char *buffer);

#endif