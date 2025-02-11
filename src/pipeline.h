#ifndef PIPELINE_H
#define PIPELINE_H

#include "cpu.h"
#include "ram.h"
#include "cache.h"
#include "control_unit.h"

typedef struct cache_item {
    bool is_cached;
    unsigned short int reg_index;
    unsigned short int result;
} cache_item;

cache_item* init_c_item();
void verify_cache_instruction(cpu* cpu, unsigned short int index_core, cache2* cache_table, char* instruction, cache_item* c_i);
char* instruction_fetch(cpu* cpu, char* program, unsigned short int index_core);
type_of_instruction instruction_decode(char* instruction, unsigned short int num_instruction);
void execute(cpu* cpu, char* program, instruction_processor* instr_processor, unsigned short int index_core, cache2** cache_table);
void memory_access(cpu* cpu, ram* memory_ram, process_control_block* pcb, type_of_instruction type, char* instruction, unsigned short int index_core);
void write_back(cpu* cpu, type_of_instruction type, process_control_block* pcb, char* instruction, unsigned short int result, unsigned short int index_core);

#endif