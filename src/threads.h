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
void log_start(process* proc);
void initialize_log_e_file();
void log_end(process* proc);
process* dequeue_process(queue_start* queue);
bool queue_is_empty(queue_start* queue);
void* thread_function(void *args);
void init_threads(cpu* cpu, ram *memory_ram, queue_start *queue_start, queue_end* queue_end);

#endif