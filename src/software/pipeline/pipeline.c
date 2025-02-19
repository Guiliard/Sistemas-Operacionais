#include "pipeline.h"

char* instruction_fetch(cpu* cpu, char* program, unsigned short int index_core) {
    char* instruction = get_line_of_program(program, cpu->core[index_core].PC);
    cpu->core[index_core].PC++;

    return instruction;
}

type_of_instruction instruction_decode(char* instruction) {
    type_of_instruction type = get_type_of_instruction(instruction);

    if (type == INVALID) {
        exit(1);    
    } else {
        return type;
    }
}

void execute(cpu* cpu, char* program, instruction_processor * instr_processor, unsigned short int index_core, cache* cache_table, type_policy policy_type) {
    control_unit(cpu, program, instr_processor, index_core, cache_table, policy_type);
}

void memory_access(cpu* cpu, ram* memory_ram, process_control_block* pcb, type_of_instruction type, char* instruction, unsigned short int index_core) {
    if (type == LOAD) {
        load(cpu, instruction, pcb, index_core);
    } else if (type == STORE) {
        store(cpu, memory_ram, pcb, instruction, index_core);
    } else {
        // do nothing
    }
}

void write_back(cpu* cpu, type_of_instruction type, process_control_block* pcb, char* instruction, unsigned short int result, unsigned short int index_core) {

    char* instruction_copy = strdup(instruction);
    unsigned short int register_index;

    if (type == ADD || type == SUB || type == MUL || type == DIV) {

        strtok(instruction_copy, " "); 

        char* register_name = strtok(NULL, " "); 

        trim(register_name);

        register_index = get_register_index(register_name);

        cpu->core[index_core].registers[register_index] = result;

        pcb->in_p->regs[register_index] = result;

    } else if (type == LOAD || type == STORE || type == LOOP || type == L_END||type == IF||type == I_END || type == ELSE || type == ELS_END) {
        // do nothing
    } else {
        printf("Error: Unrecognized instruction\n");
        exit(1);
    }
}