#ifndef THREADS_H
#define THREADS_H
#include "architecture.h"

typedef struct {
    cpu* cpu;
    ram* memory_ram;
    char* program;  // Programa a ser executado
    int core_id;    // ID do núcleo associado
} thread_args;

void* thread_function(void* args);
void init_threads(cpu* cpu, ram* memory_ram, queue_start* queue_start);

#endif