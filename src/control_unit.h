#ifndef CONTROL_UNIT_H
#define CONTROL_UNIT_H

#include "libs.h"
#include "ram.h"
#include "cpu.h"
#include "interpreter.h"

typedef struct instruction_processor {
    char *instruction;
    type_of_instruction type;
    unsigned short int num_instruction;
    unsigned short int result;
    unsigned short int loop_start;
    unsigned short int loop_value;
    bool loop;
    bool has_if;
    bool valid_if;
    bool running_if;
} instruction_processor;

void control_unit(cpu* cpu, ram* memory_ram, instruction_processor* instr_processor, unsigned short int nump);
unsigned short int ula(unsigned short int operating_a, unsigned short int operating_b, type_of_instruction operation);

unsigned short int get_register_index(char* reg_name);
unsigned short int verify_address(ram* memory_ram, char* address, unsigned short int num_positions);

void load(cpu* cpu, char* instruction, unsigned short int nump);
void store(cpu* cpu, ram* memory_ram, char* instruction, unsigned short int nump);

unsigned short int add(cpu* cpu, char* instruction, unsigned short int nump);
unsigned short int sub(cpu* cpu, char* instruction, unsigned short int nump);
unsigned short int mul(cpu* cpu, char* instruction, unsigned short int nump);
unsigned short int div_c(cpu* cpu, char* instruction, unsigned short int nump);

void if_i(cpu* cpu, ram* memory_ram, instruction_processor* instr_processor, unsigned short int nump);
void if_end(instruction_processor* instr_processor);
void else_i(cpu* cpu, ram* memory_ram, instruction_processor* instr_processor, unsigned short int nump);
void else_end(instruction_processor* instr_processor);
void loop(cpu* cpu, instruction_processor* instr_processor, unsigned short int nump);
void loop_end(cpu* cpu, instruction_processor* instr_processor, unsigned short int nump);

#endif