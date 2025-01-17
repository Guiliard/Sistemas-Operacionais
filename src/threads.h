#ifndef THREADS_H
#define THREADS_H
#include "architecture.h"
#include "scheduler.h"

typedef struct tread_args {
    cpu *cpu;
    ram *memory_ram;
    unsigned short int core_id;    // ID do núcleo associado
    queue_start* queue_start;
    process* assigned_process;
} thread_args;

void initialize_log_s_file();
void log_start(process* proc);

void* core_function(void *args);
void init_threads(cpu* cpu, ram *memory_ram, queue_start *queue_start);

#endif