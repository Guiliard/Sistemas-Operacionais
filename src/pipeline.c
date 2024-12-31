#include "pipeline.h"

char* instruction_fetch(cpu* cpu, ram* memory, unsigned short int index_core) {
    char* instruction = get_line_of_program(memory->vector, cpu->core[index_core].PC);
    cpu->core[index_core].PC++;

    return instruction;
}

type_of_instruction instruction_decode(char* instruction, unsigned short int num_instruction) {
    type_of_instruction type = verify_instruction(instruction, num_instruction);

    if (type == INVALID) {
        exit(1);    
    } else {
        return type;
    }
}

void execute(cpu* cpu, ram* memory_ram, instruction_processor * instr_processor, unsigned short int index_core) {
    control_unit(cpu, memory_ram, instr_processor, index_core);
}

void memory_access(cpu* cpu, ram* memory_ram, type_of_instruction type, char* instruction, unsigned short int index_core) {
    if (type == LOAD) {
        load(cpu, instruction, index_core);
    } else if (type == STORE) {
        store(cpu, memory_ram, instruction, index_core);
    } else {
        // do nothing
    }
}

void write_back(cpu* cpu, type_of_instruction type, char* instruction, unsigned short int result, unsigned short int index_core) {

    char* instruction_copy = strdup(instruction);

    if (type == ADD || type == SUB || type == MUL || type == DIV) {

        strtok(instruction_copy, " "); 
        char* register_name = strtok(NULL, " "); 

        cpu->core[index_core].registers[get_register_index(register_name)] = result;

    } else if (type == LOAD || type == STORE || type == LOOP || type == L_END||type == IF||type == I_END || type == ELSE || type == ELS_END) {
        // do nothing
    } else {
        printf("Error: Unrecognized instruction\n");
        exit(1);
    }
}