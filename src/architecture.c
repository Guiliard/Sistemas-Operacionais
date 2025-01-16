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

void init_pipeline(cpu* cpu, ram* memory_ram, char* program, process_control_block* pcb, unsigned short int core_number) {   
    unsigned short int num_lines = 0;

    num_lines = count_lines(program);

    printf("Number of instructions: %d - Core used: %d\n", num_lines, core_number);

    if (pcb->in_p->num_instruction == num_lines) {
        pcb->is_terminated = true;
        pcb->is_running = false;
        reset_cpu(cpu, core_number);
    }
    else {
        printf("Index_core: %d - Num_instruction: %d\n", core_number, pcb->in_p->num_instruction);

        pcb->in_p->instruction = instruction_fetch(cpu, program, core_number);

        pcb->in_p->type = instruction_decode(pcb->in_p->instruction, pcb->in_p->num_instruction);

        execute(cpu, program, pcb->in_p, core_number);

        memory_access(cpu, memory_ram, pcb, pcb->in_p->type, pcb->in_p->instruction, core_number);

        write_back(cpu, pcb->in_p->type, pcb->in_p->instruction, pcb->in_p->result, core_number);

        print_in_p(pcb->in_p);

        pcb->quantum_remaining--;
    }

}

void free_architecture(cpu* cpu, ram* memory_ram, disc* memory_disc, peripherals* peripherals, queue_start* queue_start, queue_end* queue_end, queue_block* queue_block) {
    free(cpu);
    free(memory_ram);
    free(memory_disc);
    free(peripherals);
    free(queue_start);
    free(queue_end);
    free(queue_block);
}