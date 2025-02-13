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
    pcb->waiting_resource = false;
    pcb->resource_name = NULL; 
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

void print_pcb(process_control_block* pcb) {
    printf("Process ID: %hd\n", pcb->process_id);
    printf("State: %s\n", print_enum_state(pcb->state_of_process));
    printf("Priority: %hd\n", pcb->priority);
    printf("Quantum Remaining: %hd\n", pcb->quantum_remaining);
    printf("Base Address: %hd\n", pcb->base_address);
    printf("Limit of Memory: %hd\n", pcb->limit_of_memory);
    printf("Bank of Register Used: %s\n", pcb->bank_of_register_used);
    printf("Result of Process: %s\n", pcb->result_of_process);
    printf("Waiting Resource: %d\n", pcb->waiting_resource);
    printf("Resource Name: %s\n", pcb->resource_name);
    printf("Is Terminated: %d\n", pcb->is_terminated);
    printf("Is Running: %d\n", pcb->is_running);
    printf("\n");
}

void print_in_p(instruction_processor* in_p) {
    printf("has if: %d\n", in_p->has_if);
    printf("instruction: %s\n", in_p->instruction);
    printf("loop number: %d\n", in_p->loop);
    printf("where loop start: %hd\n", in_p->loop_start);
    printf("loop limit: %hd\n", in_p->loop_value);
    printf("num instruction: %hd\n", in_p->num_instruction);
    printf("result: %hd\n", in_p->result);
    printf("is if running: %d\n", in_p->running_if);
    printf("valid_if %d\n", in_p->valid_if);
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

void add_resource_to_pcb(process_control_block *pcb, char *memory_adress) {
    if (pcb->resource_name == NULL) {
        size_t length = strlen(memory_adress) + 3; 
        pcb->resource_name = (char *)malloc(length);

        if (pcb->resource_name == NULL) {
            printf("Error: memory allocation failed in bank of register used\n");
            exit(1);
        }

        snprintf(pcb->resource_name, length, "%s, ", memory_adress);
    } else {

        size_t bank_length = strlen(pcb->resource_name);
        char *pattern = (char *)malloc(strlen(memory_adress) + 3);
        snprintf(pattern, strlen(memory_adress) + 3, "%s, ", memory_adress);

        if (strstr(pcb->resource_name, pattern) == NULL) {
            size_t new_length = bank_length + strlen(memory_adress) + 3; 
            char *new_memory = (char *)realloc(pcb->resource_name, new_length);

            if (new_memory == NULL) {
                printf("Error: memory allocation failed in resource name\n");
                free(pattern);
                exit(1);
            }

            pcb->resource_name = new_memory;
            strcat(pcb->resource_name, memory_adress);
            strcat(pcb->resource_name, ", ");
        }

        free(pattern);
    }
}