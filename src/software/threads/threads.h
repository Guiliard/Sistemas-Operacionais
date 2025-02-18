#ifndef THREADS_H
#define THREADS_H

#include "../../system/architecture.h"

typedef struct thread_args {
    cpu *cpu;
    ram *memory_ram;
    unsigned short int core_id;
    process* process_queue;
} thread_args;

bool avaliable_process(process* process_queue);
process *get_process(cpu* cpu, process* process_queue, unsigned short int core_id);
void* core_function(void *args);
void init_threads(cpu* cpu, ram *memory_ram, process* process_queue);

#endif