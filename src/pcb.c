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
    pcb->core_number = 0;
    pcb->quantum_remaining = 50;
    pcb->base_address = 0;
    pcb->limit_of_memory = 250;
    pcb->bank_of_register_used = NULL; 
    pcb->result_of_process = NULL;
    pcb->waiting_resource = false;
    pcb->resource_name = NULL; 
    pcb->is_terminated = false;
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
    
    return in_p;
}

void print_pcb(process_control_block* pcb) {
    printf("Process ID: %hd\n", pcb->process_id);
    printf("State: %s\n", print_enum_state(pcb->state_of_process));
    printf("Priority: %hd\n", pcb->priority);
    printf("Core Number: %hd\n", pcb->core_number);
    printf("Quantum Remaining: %hd\n", pcb->quantum_remaining);
    printf("Base Address: %hd\n", pcb->base_address);
    printf("Limit of Memory: %hd\n", pcb->limit_of_memory);
    printf("Bank of Register Used: %s\n", pcb->bank_of_register_used);
    printf("Result of Process: %s\n", pcb->result_of_process);
    printf("Waiting Resource: %d\n", pcb->waiting_resource);
    printf("Resource Name: %s\n", pcb->resource_name);
    printf("Is Terminated: %d\n", pcb->is_terminated);
    printf("\n");
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