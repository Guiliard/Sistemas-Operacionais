#ifndef THREADS_H
#define THREADS_H
#include "architecture.h"
#include "scheduler.h"

typedef struct thread_args {
    cpu *cpu;
    ram *memory_ram;
    unsigned short int core_id;    // ID do núcleo associado
    queue_start* queue_start;
} thread_args;

void initialize_log_s_file();
void log_start(process* proc);
void initialize_log_b_file();
void log_block(process* proc);
bool avaliable_process(queue_start *queue);
process *get_process(cpu* cpu, queue_start *queue, unsigned short int core_id);
void* core_function(void *args);
void init_threads(cpu* cpu, ram *memory_ram, queue_start *queue_start);

#endif