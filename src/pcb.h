#ifndef PCB_H
#define PCB_H

#include "libs.h"

typedef enum state {
    RUNNING,
    READY,
    BLOCK
} state;

typedef struct process_control_block {
    unsigned short int process_id;           
    state state_of_process; // RUNNING, READY, BLOCK
    unsigned short int priority;             
    unsigned short int quantum_remaining;    
    unsigned short int base_address;         
    unsigned short int limit_of_memory;
    char* bank_of_register_used;         
    bool waiting_resource;    
    char* resource_name;   
    bool is_terminated;
} process_control_block;

process_control_block* init_pcb();

void print_pcb(process_control_block* pcb);

char* print_enum_state(state state);

#endif