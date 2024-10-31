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

void init_pipeline(cpu* cpu, ram* memory_ram) {
    char* instruction;
    unsigned short int num_instruction = 0, num_lines = 0, result, loop;
    type_of_instruction type;

    num_lines = count_lines(memory_ram->vector);

    printf("Number of instructions: %d\n", num_lines);

    while (num_instruction < num_lines) {

        instruction = instruction_fetch(cpu, memory_ram);

        printf("Instruction %d: %s\n", num_instruction, instruction);

        if (type == 8) {
            loop = result;

            type = instruction_decode(instruction, num_instruction);

            printf("Type of instruction: %d\n", type);

            for (int i=0; i<loop; i++) {
                if (i==0)
                    result = execute(cpu, type, instruction);
                else
                    result += execute(cpu, type, instruction);
            }

            printf("Result: %d\n", result);
        }
        else {
            type = instruction_decode(instruction, num_instruction);

            printf("Type of instruction: %d\n", type);

            result = execute(cpu, type, instruction);

            printf("Result: %d\n", result);
        }

        memory_access(cpu, memory_ram, type, instruction);

        write_back(cpu, type, instruction, result);

        num_instruction++;
    }
}