#ifndef QUEUES_H
#define QUEUES_H

#define NUM_PROGRAMS 4

#include "libs.h"
#include "ram.h"
#include "pcb.h"
#include "reader.h"
#include "interpreter.h"

typedef struct process {
    char *program;
    process_control_block *pcb;
} process;

typedef struct queue_start {
    process *initial_queue;
} queue_start;

typedef struct queue_end {
    process *final_queue;
} queue_end;

typedef struct queue_block {
    process *block_queue;
} queue_block;

void init_queue_start(queue_start* initial_queue);
void init_queue_end(queue_end* final_queue);
void init_queue_block(queue_block* block_queue);

void populate_queue_start(queue_start* initial_queue, ram* memory_ram);

void check_resources_on_queue_start(queue_start* initial_queue);
void add_resource_to_pcb(process_control_block *pcb, char *memory_adress);

void add_process_to_queue_start(queue_start* initial_queue, process* process);
void add_process_to_queue_end(queue_end* final_queue, process* process);
void add_process_to_queue_block(queue_block* block_queue, process* process);

void remove_process_from_queue_start(queue_start* initial_queue, process* process, bool destiny_to_go); // destiny_to_go = true -> block_queue, false -> final_queue
void remove_process_from_queue_block(queue_block* block_queue, queue_start* initial_queue, process* process);

void print_queue_start(queue_start* initial_queue);
void print_queue_end(queue_end* final_queue);

#endif