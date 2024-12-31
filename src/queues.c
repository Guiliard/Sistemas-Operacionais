#include "queues.h"

void init_queue_start(queue_start* queue) {

    queue->initial_queue = malloc(NUM_PROGRAMS * sizeof(process));

    if (queue->initial_queue == NULL) {
        printf("Error: memory allocation failed in queue_start\n");
        exit(1);
    }

    for (unsigned short int i = 0; i < NUM_PROGRAMS; i++) {
        queue->initial_queue[i].program = NULL;
        queue->initial_queue[i].pcb = *init_pcb();  
    }
}

void init_queue_end(queue_end* queue) {
    queue->final_queue = malloc(NUM_PROGRAMS * sizeof(process*));
    if (queue->final_queue == NULL) {
        printf("Error: memory allocation failed in queue_end\n");
        exit(1);
    }

    for (unsigned short int i = 0; i < NUM_PROGRAMS; i++) {
        queue->final_queue[i].program = NULL;
    }
}

void init_queue_block(queue_block* queue) {
    queue->block_queue = malloc(NUM_PROGRAMS * sizeof(process*));
    if (queue->block_queue == NULL) {
        printf("Error: memory allocation failed in queue_block\n");
        exit(1);
    }

    for (unsigned short int i = 0; i < NUM_PROGRAMS; i++) {
        queue->block_queue[i].program = NULL;
    }
}

void populate_queue_start(queue_start* queue, ram* memory_ram) {
    
    char* programs_on_ram = strdup(memory_ram->vector); 
    
    if (!programs_on_ram) {
        printf("Memory allocation failed for programs on ram\n");
        exit(1);
    }

    char* only_process = strtok(programs_on_ram, "#"); 
    unsigned short int i = 0;

    while (only_process != NULL && i < NUM_PROGRAMS) {
        
        while (only_process[0] == ' ' || only_process[0] == '\n' || only_process[0] == '\t') {
            only_process++;
        }

        queue->initial_queue[i].program = malloc(strlen(only_process) + 1);

        if (queue->initial_queue[i].program == NULL) {
            printf("Memory allocation failed for program %d\n", i);
            exit(1);
        }
        
        strcpy(queue->initial_queue[i].program, only_process);

        queue->initial_queue[i].pcb.process_id = i;
        queue->initial_queue[i].pcb.priority = i;
        queue->initial_queue[i].pcb.base_address = (unsigned long)(only_process - programs_on_ram);

        i++;
        only_process = strtok(NULL, "#"); 
    }

    free(programs_on_ram); 
}

void print_queue_start(queue_start* queue) {
    for (unsigned short int i = 0; i < NUM_PROGRAMS; i++) {
        if (queue->initial_queue[i].program != NULL) {
            printf("Program %d:\n%s\n", i, queue->initial_queue[i].program);
            print_pcb(queue->initial_queue[i].pcb);
        }
    }
}