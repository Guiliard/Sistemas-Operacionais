#include "architecture.h"

void init_architecture(cpu* cpu, ram* memory_ram, disc* memory_disc, peripherals* peripherals) {
    init_cpu(cpu);
    init_ram(memory_ram);
    init_disc(memory_disc);
    init_peripherals(peripherals);
}

void load_program_on_ram(ram* memory_ram, char* program) {
    unsigned short int num_caracters = strlen(program);

    for (unsigned short int i = 0; i < num_caracters; i++) {
        memory_ram->vector[i] = program[i];
    }
}

void check_instructions_on_ram(ram* memory_ram) {
    
    char* line;
    unsigned short int num_line = 0;
    unsigned short int num = count_lines(memory_ram->vector);

    while (num_line < num) {
        line = get_line_of_program(memory_ram->vector, num_line);
        verify_instruction(line, num_line);
        num_line++;
    }
}

void pipiline(cpu* cpu, ram* memory) {
    char* instruction;
    unsigned short int num_instruction = 0;

    instruction = instruction_fetch(cpu, memory);

    instruction_decode(instruction, num_instruction);

    // execute(cpu);

    // memory_access(cpu, memory);

    // write_back(cpu);
}

char* instruction_fetch(cpu* cpu, ram* memory) {
    char* instruction = get_line_of_program(memory->vector, cpu->core[0].PC);
    cpu->core[0].PC++;

    return instruction;
}

void instruction_decode(char* instruction, unsigned short int num_instruction) {
    type_of_instruction type = verify_instruction(instruction, num_instruction);

    if (type == INVALID) {
        exit(1);    
    } 
}
