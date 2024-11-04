#include "pipeline.h"

char* instruction_fetch(cpu* cpu, ram* memory) {
    char* instruction = get_line_of_program(memory->vector, cpu->core[0].PC);
    cpu->core[0].PC++;

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

void execute(cpu* cpu, pipe *p) {
    control_unit(cpu, p);
}

void memory_access(cpu* cpu, ram* memory_ram, type_of_instruction type, char* instruction) {
    if (type == LOAD) {
        load(cpu, instruction);
    } else if (type == STORE) {
        store(cpu, memory_ram, instruction);
    } else {
        // do nothing
    }
}

void write_back(cpu* cpu, type_of_instruction type, char* instruction, unsigned short int result) {

    char* instruction_copy = strdup(instruction);

    if (type == ADD || type == SUB || type == MUL || type == DIV) {

        strtok(instruction_copy, " "); 
        char* register_name = strtok(NULL, " "); 

        cpu->core[0].registers[get_register_index(register_name)] = result;

    } else if (type == LOAD || type == STORE || type == LOOP || type == L_END||type == IF||type == I_END || type == ELSE || type == ELS_END) {
        // do nothing
    } else {
        printf("Error: Unrecognized instruction\n");
        exit(1);
    }
}
