#include "architecture.h"

void init_architecture(cpu* cpu, ram* memory_ram, disc* memory_disc, peripherals* peripherals, process* process_queue) {
    init_cpu(cpu);
    init_ram(memory_ram);
    init_disc(memory_disc);
    init_peripherals(peripherals);
    init_process_queue(process_queue);
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

void init_pipeline(cpu* cpu, ram* memory_ram, process* process, unsigned short int core_number, cache* cache_table) {   
    unsigned short int num_lines = 0;
    instruction_cache_item* inst_cache_item = init_instruction_cache_item();

    num_lines = count_lines(process->program);

    if (process->pcb->in_p->num_instruction == num_lines) {
        printf("Core %hd finalizou o processo id: %hd\n", core_number, process->pcb->process_id);
        write_logs_end_file(process);
        process->pcb->is_terminated = true;
        process->pcb->is_running = false;
        process->pcb->state_of_process = READY;
        reset_cpu(cpu, core_number);
        process->pcb->in_p->num_instruction++;
    }
    else {
        process->pcb->in_p->instruction = instruction_fetch(cpu, process->program, core_number);

        verify_cache_instruction(cpu, core_number, cache_table, process->pcb->in_p->instruction, inst_cache_item);

        if (inst_cache_item->is_cached) {
            cpu->core[core_number].registers[inst_cache_item->reg_index] = inst_cache_item->result;
            process->pcb->in_p->regs[inst_cache_item->reg_index] = inst_cache_item->result;
            process->pcb->in_p->num_instruction++;
            printf("cache find %s\n",process->pcb->in_p->instruction);
        } else {
            process->pcb->in_p->type = instruction_decode(process->pcb->in_p->instruction, process->pcb->in_p->num_instruction);

            execute(cpu, process->program, process->pcb->in_p, core_number, cache_table);

            memory_access(cpu, memory_ram, process->pcb, process->pcb->in_p->type, process->pcb->in_p->instruction, core_number);

            write_back(cpu, process->pcb->in_p->type, process->pcb, process->pcb->in_p->instruction, process->pcb->in_p->result, core_number);
        }
        inst_cache_item->is_cached = false;
        process->pcb->quantum_remaining--;
    }
}

void update_regs(cpu* cpu, process_control_block* pcb, unsigned short int core_number) {
    for (int i=0; i<NUM_REGISTERS; i++) {
        cpu->core[core_number].registers[i] = pcb->in_p->regs[i];
    }
    cpu->core[core_number].PC = pcb->in_p->num_instruction;
}

void free_architecture(cpu* cpu, ram* memory_ram, disc* memory_disc, peripherals* peripherals, process* process_queue) {
    free(cpu);
    free(memory_ram);
    free(memory_disc);
    free(peripherals);
    free(process_queue);
}