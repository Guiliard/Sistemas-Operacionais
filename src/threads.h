#ifndef THREADS_H
#define THREADS_H
#include "architecture.h"

typedef struct tread_args {
    cpu *cpu;
    ram *memory_ram;
    process *process;  // Programa a ser executado
    int core_id;    // ID do núcleo associado
    queue_end** queue_end_ptr;
} thread_args;

void* thread_function(void *args);
void init_threads(cpu* cpu, ram *memory_ram, queue_start *queue_start, queue_end** queue_end);

#endif