#include "pcb.h"

process_control_block* init_pcb() {
    process_control_block* pcb = malloc(sizeof(process_control_block));
    if (pcb == NULL) {
        printf("Memory allocation failed for PCB\n");
        exit(1);
    }

    pcb->process_id = 0;
    pcb->state_of_process = RUNNING;  
    pcb->priority = 0;
    pcb->quantum_remaining = 10;
    pcb->total_quantum = 10;
    pcb->base_address = 0;
    pcb->limit_of_memory = 250;
    pcb->on_core = -1;
    pcb->bank_of_register_used = NULL; 
    pcb->result_of_process = NULL;
    pcb->is_terminated = false;
    pcb->is_running = false;
    pcb->is_blocked = false;
    pcb->in_p = init_in_p();

    return pcb;
}

instruction_processor* init_in_p() {
    instruction_processor* in_p = malloc(sizeof(instruction_processor));
    if (in_p == NULL) {
        printf("Memory allocation failed for PCB\n");
        exit(1);
    }

    in_p->has_if = false;
    in_p->loop = false;
    in_p->loop_start = 0;
    in_p->loop_value = 0;
    in_p->num_instruction = 0;
    in_p->result = 0;
    in_p->running_if = false;
    in_p->valid_if = false;
    for (int i = 0; i < NUM_REGISTERS; i++) {
        in_p->regs[i] = 0;
    }
    
    return in_p;
}

void write_pcb_to_file(FILE* file, process_control_block* pcb) {
    fprintf(file, "Process ID: %hd\n", pcb->process_id);
    fprintf(file, "State: %s\n", print_enum_state(pcb->state_of_process));
    fprintf(file, "Priority: %hd\n", pcb->priority);
    fprintf(file, "Quantum Remaining: %hd\n", pcb->quantum_remaining);
    fprintf(file, "Base Address: %hd\n", pcb->base_address);
    fprintf(file, "Limit of Memory: %hd\n", pcb->limit_of_memory);
    fprintf(file, "Bank of Register Used: %s\n", pcb->bank_of_register_used);
    fprintf(file, "Result of Process: %s\n", pcb->result_of_process);
    fprintf(file, "Is Terminated: %d\n", pcb->is_terminated);
    fprintf(file, "Is Running: %d\n", pcb->is_running);
    fprintf(file, "Is Blocked: %d", pcb->is_blocked);
}

char* print_enum_state(state state) {
    switch (state) {
        case RUNNING:
            return "RUNNING";
        case READY:
            return "READY";
        case BLOCK:
            return "BLOCK";
        default:
            return "UNKNOWN";
    }
}