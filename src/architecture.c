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
    pipe p;
    unsigned short int num_lines = 0;
    p.num_instruction = 0;
    p.mem_ram = memory_ram;

    num_lines = count_lines(memory_ram->vector);

    printf("Number of instructions: %d\n", num_lines);

    while (p.num_instruction < num_lines) {

        p.instruction = instruction_fetch(cpu, memory_ram);

        p.type = instruction_decode(p.instruction, p.num_instruction);

        execute(cpu, &p);

        memory_access(cpu, memory_ram, p.type, p.instruction);

        write_back(cpu, p.type, p.instruction, p.result);

    }
}