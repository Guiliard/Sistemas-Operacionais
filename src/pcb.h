#ifndef PCB_H
#define PCB_H

#include "libs.h"
#include "interpreter.h"
#include "cpu.h"

typedef enum state {
    RUNNING,
    READY,
    BLOCK
} state;

typedef struct instruction_processor {
    char *instruction;
    type_of_instruction type;
    unsigned short int num_instruction;
    unsigned short int result;
    unsigned short int loop_start;
    unsigned short int loop_value;
    bool loop;
    bool has_if;
    bool valid_if;
    bool running_if;
    unsigned short int regs[NUM_REGISTERS];
} instruction_processor;

typedef struct process_control_block {
    unsigned short int process_id;           
    state state_of_process; // RUNNING, READY, BLOCK
    unsigned short int priority;
    unsigned short int total_quantum;  
    unsigned short int quantum_remaining;    
    unsigned short int base_address;         
    unsigned short int limit_of_memory;
    int on_core;
    char* bank_of_register_used;     
    char* result_of_process;    
    bool waiting_resource;    
    char* resource_name;   
    bool is_terminated;
    bool is_running;
    bool is_blocked;
    instruction_processor* in_p;
} process_control_block;

process_control_block* init_pcb();
instruction_processor* init_in_p();

void print_pcb(process_control_block* pcb);
void print_in_p(instruction_processor* in_p);

char* print_enum_state(state state);

#endif