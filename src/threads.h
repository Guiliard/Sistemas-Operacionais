#ifndef THREADS_H
#define THREADS_H
#include "architecture.h"

typedef struct tread_args {
    cpu *cpu;
    ram *memory_ram;
    process *process;  // Programa a ser executado
    unsigned short int core_id;    // ID do núcleo associado
    queue_start* queue_start;
    queue_end* queue_end;
} thread_args;

int compare_priority(const void* a, const void* b);

void initialize_log_s_file();
void initialize_log_e_file();
void initialize_log_b_file();

void log_start(process* proc);
void log_end(process* proc);
void log_block(process* proc);

void* thread_function(void *args);
void init_threads(cpu* cpu, ram *memory_ram, queue_start *queue_start, queue_end* queue_end, queue_block* queue_block);
void release_resources_and_notify(queue_start* queue_start, process* finished_process);

void compare_resource_of_process_in_queue_start(queue_start* initial_queue, queue_block* queue_block, short int index);

bool has_shared_resource(char *process_1, char *process_2);

#endif