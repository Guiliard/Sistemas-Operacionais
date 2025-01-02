#include "threads.h"

void* thread_function(void* args) {
    thread_args* t_args = (thread_args*)args;

    printf("Core %d: Iniciando execução do programa:\n%s\n", t_args->core_id, t_args->program);

    init_pipeline(t_args->cpu, t_args->memory_ram, t_args->program, t_args->core_id);

    printf("Core %d: Execução finalizada.\n", t_args->core_id);

    pthread_exit(NULL);
}

void init_threads(cpu* cpu, ram* memory_ram, queue_start* queue_start) {
    pthread_t threads[NUM_PROGRAMS];
    thread_args t_args[NUM_PROGRAMS];

    for (int i = 0; i < NUM_PROGRAMS; i++) {
        t_args[i].cpu = cpu;
        t_args[i].memory_ram = memory_ram;
        t_args[i].program = queue_start->initial_queue[i].program;
        t_args[i].core_id = i % NUM_CORES;

        if (pthread_create(&threads[i], NULL, thread_function, &t_args[i]) != 0) {
            perror("Error: Fail on creating thread");
            exit(1);
        }
    }

    for (int i = 0; i < NUM_PROGRAMS; i++) {
        pthread_join(threads[i], NULL);
    }
}