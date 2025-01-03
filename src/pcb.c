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
    pcb->quantum_remaining = 50;
    pcb->base_address = 0;
    pcb->limit_of_memory = 250;
    pcb->bank_of_register_used = NULL; 
    pcb->waiting_resource = false;
    pcb->resource_name = NULL; 

    return pcb;
}

void print_pcb(process_control_block* pcb) {
    printf("Process ID: %d\n", pcb->process_id);
    printf("State: %s\n", print_enum_state(pcb->state_of_process));
    printf("Priority: %d\n", pcb->priority);
    printf("Quantum Remaining: %d\n", pcb->quantum_remaining);
    printf("Base Address: %d\n", pcb->base_address);
    printf("Limit of Memory: %d\n", pcb->limit_of_memory);
    printf("Bank of Register Used: %s\n", pcb->bank_of_register_used);
    printf("Waiting Resource: %d\n", pcb->waiting_resource);
    printf("Resource Name: %s\n", pcb->resource_name);
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