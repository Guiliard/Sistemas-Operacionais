#include "threads.h"

pthread_mutex_t memory_mutex;
pthread_mutex_t cpu_mutex;
sem_t core_semaphores[NUM_CORES]; 


int compare_priority(const void* a, const void* b) {
    const process_control_block* pcb1 = (const process_control_block*)a;
    const process_control_block* pcb2 = (const process_control_block*)b;

    if (pcb1->priority < pcb2->priority) {
        return -1;
    } else if (pcb1->priority > pcb2->priority) {
        return 1;
    } else {
        return 0;
    }
}

void initialize_log_s_file() {
    FILE* file = fopen("output/start.txt", "w");  
    if (file == NULL) {
        perror("Error: Cannot create output/start.txt");
        exit(1);  
    }
    fprintf(file, "Queue of programs to be executed.\n\n");
    fclose(file);
}

void initialize_log_e_file() {
    FILE* file = fopen("output/end.txt", "w");  
    if (file == NULL) {
        perror("Error: Cannot create output/end.txt");
        exit(1);  
    }
    fprintf(file, "Queue of done executed programs.\n\n");
    fclose(file);
}

void log_start(process* proc) {
    FILE* file = fopen("output/start.txt", "a");  
    if (file == NULL) {
        perror("Error: Cannot open output/start.txt");
        return;
    }

    fprintf(file, "------------------PROGRAM %hd------------------\n", proc->pcb->process_id);
    fprintf(file, "%s\n", proc->program);
    fprintf(file, "PCB of process: %hd/ State: %s/ Priority: %hd\n",
    proc->pcb->process_id,print_enum_state(proc->pcb->state_of_process),proc->pcb->priority);
    fprintf(file, "Quantum remaining: %hd/ Base address: %hd/ Memory limit: %hd\n",
    proc->pcb->quantum_remaining,proc->pcb->base_address,proc->pcb->limit_of_memory);
    fprintf(file, "Used resources: %s\n\n",
    proc->pcb->resource_name);

    fclose(file);  
}

void log_end(process* proc) {
    FILE* file = fopen("output/end.txt", "a");  
    if (file == NULL) {
        perror("Error: Cannot open output/end.txt");
        return;
    }

    fprintf(file, "------------------PROGRAM %hd------------------\n", proc->pcb->process_id);
    fprintf(file, "%s\n", proc->program);
    fprintf(file, "PCB of process: %hd/ State: %s/ Priority: %hd\n",
    proc->pcb->process_id,print_enum_state(proc->pcb->state_of_process),proc->pcb->priority);
    fprintf(file, "Quantum remaining: %hd/ Base address: %hd/ Memory limit: %hd\n",
    proc->pcb->quantum_remaining,proc->pcb->base_address,proc->pcb->limit_of_memory);
    fprintf(file, "Used registers: %s\n",
    proc->pcb->bank_of_register_used);
    fprintf(file, "Result of process: %s\n\n",
    proc->pcb->result_of_process);

    fclose(file);  
}

void* thread_function(void* args) {
    thread_args* t_args = (thread_args*)args;

    printf("Core %d: Waiting for semaphore.\n", t_args->core_id);
    sem_wait(&core_semaphores[t_args->core_id]);
    printf("Core %d: Semaphore acquired.\n", t_args->core_id);

    printf("Core %d: Iniciando execução do programa:\n%s\n", t_args->core_id, t_args->process->program);

    if (t_args->process == NULL || t_args->process->program == NULL) {
        fprintf(stderr, "Error: Invalid process or program in core %d\n", t_args->core_id);
        pthread_exit(NULL);
    }   

    pthread_mutex_lock(&cpu_mutex);
    pthread_mutex_lock(&memory_mutex);

    init_pipeline(t_args->cpu, t_args->memory_ram, t_args->process->program, t_args->process->pcb, t_args->core_id);

    pthread_mutex_unlock(&memory_mutex);
    pthread_mutex_unlock(&cpu_mutex);

    add_process_to_queue_end(t_args->queue_end, t_args->process);
    log_end(t_args->process);

    printf("Core %d: Execução finalizada.\n", t_args->core_id);

    sem_post(&core_semaphores[t_args->core_id]);

    pthread_exit(NULL);
}

void init_threads(cpu* cpu, ram* memory_ram, queue_start* queue_start, queue_end* queue_end) {
    pthread_t threads[NUM_PROGRAMS];
    thread_args t_args[NUM_PROGRAMS];

    qsort(queue_start->initial_queue, NUM_PROGRAMS, sizeof(process), compare_priority);

    if (pthread_mutex_init(&memory_mutex, NULL) != 0 || pthread_mutex_init(&cpu_mutex, NULL) != 0) {
        perror("Error: Fail on initializing mutex");
        exit(1);
    }

    for (unsigned short int i = 0; i < NUM_CORES; i++) {
        if (sem_init(&core_semaphores[i], 0, 1) != 0) {
            perror("Error: Fail on initializing semaphore");
            exit(1);
        }
    }

    for (unsigned short int i = 0; i < NUM_PROGRAMS; i++) {
        t_args[i].cpu = cpu;
        t_args[i].memory_ram = memory_ram;
        t_args[i].process = &queue_start->initial_queue[i];
        t_args[i].core_id = i % NUM_CORES; 
        t_args[i].queue_end = queue_end;

        log_start(t_args[i].process);
        printf("index_core: %d\n", t_args[i].core_id);

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

    for (unsigned short int i = 0; i < NUM_CORES; i++) {
        sem_destroy(&core_semaphores[i]);
    }
}