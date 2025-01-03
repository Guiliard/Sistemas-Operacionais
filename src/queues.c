#include "queues.h"

unsigned short int queue__end_index = 0;

void init_queue_start(queue_start* initial_queue) {

    initial_queue->initial_queue = malloc(NUM_PROGRAMS * sizeof(process));

    if (initial_queue->initial_queue == NULL) {
        printf("Error: memory allocation failed in queue_start\n");
        exit(1);
    }

    for (unsigned short int i = 0; i < NUM_PROGRAMS; i++) {
        initial_queue->initial_queue[i].program = NULL;
        initial_queue->initial_queue[i].pcb = init_pcb();  
    }
}

void init_queue_end(queue_end* final_queue) {
    final_queue->final_queue = malloc(NUM_PROGRAMS * sizeof(process*));
    if (final_queue->final_queue == NULL) {
        printf("Error: memory allocation failed in queue_end\n");
        exit(1);
    }

    for (unsigned short int i = 0; i < NUM_PROGRAMS; i++) {
        final_queue->final_queue[i].program = NULL;
    }
}

void init_queue_block(queue_block* block_queue) {
    block_queue->block_queue = malloc(NUM_PROGRAMS * sizeof(process*));
    if (block_queue->block_queue == NULL) {
        printf("Error: memory allocation failed in queue_block\n");
        exit(1);
    }

    for (unsigned short int i = 0; i < NUM_PROGRAMS; i++) {
        block_queue->block_queue[i].program = NULL;
    }
}

void populate_queue_start(queue_start* initial_queue, ram* memory_ram) {
    
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

        initial_queue->initial_queue[i].program = malloc(strlen(only_process) + 1);

        if (initial_queue->initial_queue[i].program == NULL) {
            printf("Memory allocation failed for program %d\n", i);
            exit(1);
        }
        
        strcpy(initial_queue->initial_queue[i].program, only_process);

        initial_queue->initial_queue[i].pcb->process_id = i;
        initial_queue->initial_queue[i].pcb->priority = i;
        initial_queue->initial_queue[i].pcb->base_address = (unsigned long)(only_process - programs_on_ram);

        i++;
        only_process = strtok(NULL, "#"); 
    }

    free(programs_on_ram); 
}

void add_process_to_queue_end(queue_end** queue_end_ptr, process* new_process) {
    queue_end* queue = *queue_end_ptr;  // Desreferencia o ponteiro duplo

    // Adiciona o novo processo na posição livre
    queue->final_queue[queue__end_index] = *new_process;

    // Incrementa o índice da fila
    queue__end_index++;
}


void print_queue_start(queue_start* initial_queue) {
    printf("Initial Queue:\n");

    for (unsigned short int i = 0; i < NUM_PROGRAMS; i++) {
        if (initial_queue->initial_queue[i].program != NULL) {
            printf("Program %d:\n%s\n", i, initial_queue->initial_queue[i].program);
            print_pcb(initial_queue->initial_queue[i].pcb);
        }
    }
}

void print_queue_end(queue_end* final_queue) {
    printf("Final Queue:\n");

    for (unsigned short int i = 0; i < NUM_PROGRAMS; i++) {
        if (final_queue->final_queue[i].program != NULL) {
            printf("Program %d:\n%s\n", i, final_queue->final_queue[i].program);
            print_pcb(final_queue->final_queue[i].pcb);
        }
    }
}