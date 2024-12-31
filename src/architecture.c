#include "architecture.h"

void init_architecture(cpu* cpu, ram* memory_ram, disc* memory_disc, peripherals* peripherals, queue_start* queue_start, queue_end* queue_end, queue_block* queue_block) {
    init_cpu(cpu);
    init_ram(memory_ram);
    init_disc(memory_disc);
    init_peripherals(peripherals);
    init_queue_start(queue_start);
    init_queue_end(queue_end);
    init_queue_block(queue_block);
}

void load_program_on_ram(ram* memory_ram, char* program) {
    unsigned short int num_caracters = strlen(program);
    unsigned short int used_memory = 0;

    for (unsigned short int i = 0; i < NUM_MEMORY; i++) {
        if (memory_ram->vector[i] != '\0') {
            used_memory++;
        } else {
            break; 
        }
    }

    if (used_memory + num_caracters + 3 > NUM_MEMORY) { // 
        printf("Error: Not enough space in RAM for the program.\n");
        exit(1);
    }

    unsigned short int start_position = used_memory;

    for (unsigned short int i = 0; i < num_caracters; i++) {
        memory_ram->vector[start_position + i] = program[i];
    }

    sprintf(memory_ram->vector + start_position + num_caracters, "\n#\n");
}

void check_instructions_on_ram(ram* memory_ram) {
    
    char* line;
    unsigned short int num_line = 0;
    unsigned short int num = count_lines(memory_ram->vector);
    unsigned short int num_program = 0;
    unsigned short int num_line_of_program = 0;
    type_of_instruction type;
    
    while (num_line < num) {
        line = get_line_of_program(memory_ram->vector, num_line);

        if (strcmp(line, "#") == 0) {
            num_program++;
            num_line_of_program = 0;
        }

        type = verify_instruction(line, num_line);

        if (type == INVALID) {
            printf("Error: Line %d of program %d\n", num_line_of_program, num_program);
            exit(1);
        }

        num_line++;
        num_line_of_program++;
    }
}

void init_pipeline(cpu* cpu, ram* memory_ram, unsigned short int index_core) {
    
    instruction_processor instr_processor;
    unsigned short int num_lines = 0;
    instr_processor.num_instruction = 0;

    num_lines = count_lines(memory_ram->vector);

    printf("Number of instructions: %d\n", num_lines);

    while (instr_processor.num_instruction < num_lines) {

        instr_processor.instruction = instruction_fetch(cpu, memory_ram, index_core);

        instr_processor.type = instruction_decode(instr_processor.instruction, instr_processor.num_instruction);

        execute(cpu, memory_ram, &instr_processor, index_core);

        memory_access(cpu, memory_ram, instr_processor.type, instr_processor.instruction, index_core);

        write_back(cpu, instr_processor.type, instr_processor.instruction, instr_processor.result, index_core);
    }
}