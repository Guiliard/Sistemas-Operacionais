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

void init_queue_start(queue_start* initial_queue);

void initialize_log_e_file();
void log_end(process* proc);

void populate_queue_start(queue_start* initial_queue, ram* memory_ram);

void check_resources_on_queue_start(queue_start* initial_queue);
void add_resource_to_pcb(process_control_block *pcb, char *memory_adress);

void organize_process_of_queue_start(queue_start* initial_queue, unsigned short int process_id);

#endif