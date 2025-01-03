#include "threads.h"

pthread_mutex_t memory_mutex;
pthread_mutex_t cpu_mutex;

void* thread_function(void* args) {
    thread_args* t_args = (thread_args*)args;

    printf("Core %d: Iniciando execução do programa:\n%s\n", t_args->core_id, t_args->process->program);

    pthread_mutex_lock(&cpu_mutex);
    pthread_mutex_lock(&memory_mutex);
    
    init_pipeline(t_args->cpu, t_args->memory_ram, t_args->process->program, t_args->process->pcb, t_args->core_id);
    
    pthread_mutex_unlock(&memory_mutex);
    pthread_mutex_unlock(&cpu_mutex);

    add_process_to_queue_end(t_args->queue_end, t_args->process);

    printf("Core %d: Execução finalizada.\n", t_args->core_id);

    pthread_exit(NULL);
}

void init_threads(cpu* cpu, ram* memory_ram, queue_start* queue_start, queue_end* queue_end) {
    pthread_t threads[NUM_PROGRAMS];
    thread_args t_args[NUM_PROGRAMS];

    if (pthread_mutex_init(&memory_mutex, NULL) != 0 || pthread_mutex_init(&cpu_mutex, NULL) != 0) {
        perror("Error: Fail on initializing mutex");
        exit(1);
    }

    for (unsigned short int i = 0; i < NUM_PROGRAMS; i++) {
        t_args[i].cpu = cpu;
        t_args[i].memory_ram = memory_ram;
        t_args[i].process = &queue_start->initial_queue[i];
        t_args[i].core_id = i % NUM_CORES;
        t_args[i].queue_end = queue_end;

        if (pthread_create(&threads[i], NULL, thread_function, &t_args[i]) != 0) {
            perror("Error: Fail on creating thread");
            exit(1);
        }
    }

    for (int i = 0; i < NUM_PROGRAMS; i++) {
        pthread_join(threads[i], NULL);
    }

    pthread_mutex_destroy(&memory_mutex);
    pthread_mutex_destroy(&cpu_mutex);
}