#ifndef QUEUES_H
#define QUEUES_H

#define NUM_PROGRAMS 4

#include "libs.h"
#include "ram.h"
#include "pcb.h"

typedef struct process {
    char *program;
    process_control_block pcb;
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

void init_queue_start(queue_start* queue);
void init_queue_end(queue_end* queue);
void init_queue_block(queue_block* queue);

void populate_queue_start(queue_start* queue, ram* memory_ram);
void print_queue_start(queue_start* queue);

#endif