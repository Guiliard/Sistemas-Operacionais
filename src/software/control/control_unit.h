#ifndef CONTROL_UNIT_H
#define CONTROL_UNIT_H

#include "../../utils/libs.h"
#include "../../hardware/memory/ram.h"
#include "../../hardware/cpu/cpu.h"
#include "interpreter.h"
#include "../processes/pcb.h"
#include "../../hardware/memory/cache.h"

void control_unit(cpu* cpu, char* program, instruction_processor* instr_processor, unsigned short int index_core, cache* cache_table, type_policy policy_type);
unsigned short int ula(unsigned short int operating_a, unsigned short int operating_b, type_of_instruction operation, cache* cache_table, type_policy policy_type);

unsigned short int get_register_index(char* reg_name);
void verify_cache_instruction(cpu* cpu, unsigned short int index_core, cache* cache_table, char* instruction, instruction_cache_item* inst_cache_item);

void load(cpu* cpu, char* instruction, process_control_block* pcb, unsigned short int index_core);
void store(cpu* cpu, ram* memory_ram, process_control_block* pcb, char* instruction, unsigned short int index_core);

unsigned short int add(cpu* cpu, char* instruction, unsigned short int index_core, cache* cache_table, type_policy policy_type);
unsigned short int sub(cpu* cpu, char* instruction, unsigned short int index_core, cache* cache_table, type_policy policy_type);
unsigned short int mul(cpu* cpu, char* instruction, unsigned short int index_core, cache* cache_table, type_policy policy_type);
unsigned short int div_c(cpu* cpu, char* instruction, unsigned short int index_core, cache* cache_table, type_policy policy_type);

void if_i(cpu* cpu, char* program, instruction_processor* instr_processor, unsigned short int index_core);
void if_end(instruction_processor* instr_processor);
void else_i(cpu* cpu, char* program, instruction_processor* instr_processor, unsigned short int index_core);
void else_end(instruction_processor* instr_processor);
void loop(cpu* cpu, instruction_processor* instr_processor, unsigned short int index_core);
void loop_end(cpu* cpu, instruction_processor* instr_processor, unsigned short int index_core);

void add_register_bank_to_pcb(process_control_block *pcb, char *register_name);
void add_result_of_process_to_pcb(process_control_block *pcb, char *buffer);

#endif