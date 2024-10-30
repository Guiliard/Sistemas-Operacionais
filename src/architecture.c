#include "architecture.h"

void init_architecture(cpu* cpu, ram* memory_ram, disc* memory_disc, peripherals* peripherals) {
    init_cpu(cpu);
    init_ram(memory_ram);
    init_disc(memory_disc);
    init_peripherals(peripherals);
}

void load_program_on_ram(ram* memory_ram, char* program) {
    unsigned short int num_caracters = strlen(program);

    if (num_caracters >= NUM_MEMORY) {
        printf("Error: Program size exceeds RAM capacity.\n");
        exit(1);
    }

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
    unsigned short int num_instruction = 0, num_lines = 0;
    type_of_instruction type;

    num_lines = count_lines(memory->vector);

    while (num_instruction < num_lines) {

    instruction = instruction_fetch(cpu, memory);

    type = instruction_decode(instruction, num_instruction);

    execute(cpu, memory, type, instruction);

    num_instruction++;
    }
}

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

void execute(cpu* cpu, ram* memory_ram, type_of_instruction type, char* instruction) {
    control_unit(cpu, memory_ram, type, instruction);
}
