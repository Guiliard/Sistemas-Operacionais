#include "threads.h"

pthread_mutex_t queue_mutex;
pthread_cond_t cond_var;
int current_core = 0;
bool running_core[NUM_CORES];
bool has_process[NUM_CORES];
bool more_process[NUM_CORES];
int end_thread = 0;

void initialize_log_s_file() {
    FILE* file = fopen("output/start.txt", "w");  
    if (file == NULL) {
        perror("Error: Cannot create output/start.txt");
        exit(1);  
    }
    fprintf(file, "Queue of programs to be executed.\n\n");
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

void initialize_log_b_file() {
    FILE* file = fopen("output/block.txt", "w");  
    if (file == NULL) {
        perror("Error: Cannot create output/block.txt");
        exit(1);  
    }
    fprintf(file, "Queue of blocked programs.\n\n");
    fclose(file);
}

void log_block(process* proc) {
    FILE* file = fopen("output/block.txt", "a");  
    if (file == NULL) {
        perror("Error: Cannot open output/block.txt");
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

process *get_process(queue_start *queue) {
    pthread_mutex_lock(&queue_mutex);
    for (unsigned short int i = 0; i < NUM_PROGRAMS; i++) {      
        if (!queue->initial_queue[i].pcb->is_running && !queue->initial_queue[i].pcb->is_terminated) {
            queue->initial_queue[i].pcb->is_running = true;
            pthread_mutex_unlock(&queue_mutex);
            return &queue->initial_queue[i];
        }
    }
    pthread_mutex_unlock(&queue_mutex);
    return NULL;
}

void *core_function(void *args) {
    thread_args *t_args = (thread_args *)args;

    while (1) {
        pthread_mutex_lock(&queue_mutex);
        while (t_args->core_id != current_core && running_core[t_args->core_id]) {
            pthread_cond_wait(&cond_var, &queue_mutex);
        }
        pthread_mutex_unlock(&queue_mutex);

        process *proc = t_args->assigned_process;
        if (proc && !proc->pcb->is_terminated && !quantum_over(t_args->assigned_process)) {
            init_pipeline(t_args->cpu, t_args->memory_ram, proc, t_args->core_id, t_args->queue_end);
        }

        pthread_mutex_lock(&queue_mutex);
        if (quantum_over(t_args->assigned_process)) {
            printf("Core %d bloqueou o processo id: %hd, quantum zerado\n", t_args->core_id, t_args->assigned_process->pcb->process_id);
            has_process[t_args->core_id] = false;
            log_block(t_args->assigned_process);
            t_args->assigned_process->pcb->is_running = false;
            t_args->assigned_process->pcb->is_terminated = false;
        }
        pthread_mutex_unlock(&queue_mutex);

        pthread_mutex_lock(&queue_mutex);
        if (proc && proc->pcb->is_terminated) {
            has_process[t_args->core_id] = false;
        }
        pthread_mutex_unlock(&queue_mutex);

        pthread_mutex_lock(&queue_mutex);
        if (NUM_CORES > 1) {
            do {
                current_core = (current_core + 1) % NUM_CORES;
            } while (!running_core[current_core]);
            pthread_cond_broadcast(&cond_var);
        }
        pthread_mutex_unlock(&queue_mutex);

        if (!more_process[t_args->core_id]) {
            printf("Core %d finalizado.\n", t_args->core_id);
            break;
        }
    }

    running_core[t_args->core_id] = false;
    end_thread++;
    pthread_exit(NULL);
}

void init_threads(cpu *cpu, ram *memory_ram, queue_start *queue_start, queue_end *queue_end) {
    int cores_ativos;
    if (NUM_PROGRAMS < NUM_CORES)
        cores_ativos = NUM_PROGRAMS;
    else
        cores_ativos = NUM_CORES;
    pthread_t threads[cores_ativos];
    thread_args *t_args = malloc(sizeof(thread_args) * cores_ativos);

    pthread_mutex_init(&queue_mutex, NULL);
    pthread_cond_init(&cond_var, NULL);

    for (int i = 0; i < cores_ativos; i++) {
        t_args[i].core_id = i;
        t_args[i].cpu = cpu;
        t_args[i].memory_ram = memory_ram;
        t_args[i].queue_start = queue_start;
        t_args[i].queue_end = queue_end;
        running_core[i] = true;
        more_process[i] = true;
        t_args[i].assigned_process = get_process(queue_start);
        has_process[i] = t_args[i].assigned_process != NULL;

        if (!has_process[i]) {
            printf("Nenhum processo disponível para o core %d\n", i);
        } else {
            printf("Core %d iniciou com processo id: %hd\n", i, t_args[i].assigned_process->pcb->process_id);
        }
        log_start(t_args[i].assigned_process);

        pthread_create(&threads[i], NULL, core_function, &t_args[i]);
    }

    while (end_thread < cores_ativos) {
        for (int i = 0; i < cores_ativos; i++) {
            if (!has_process[i] && running_core[i]) {
                process* new_proc = get_process(queue_start);
                if (new_proc) {
                    update_regs(t_args->cpu, t_args[i].assigned_process->pcb, t_args->core_id);
                    t_args[i].assigned_process = new_proc;
                    log_start(t_args[i].assigned_process);
                    has_process[i] = true;
                    printf("Core %d recebeu novo processo id: %hd\n", i, t_args[i].assigned_process->pcb->process_id);
                } else {
                    //printf("Nenhum novo processo disponível para o core %d\n", t_args[i].core_id);
                    more_process[i] = false;
                }
            }
        }
        pthread_cond_broadcast(&cond_var);
    }

    for (int i = 0; i < cores_ativos; i++) {
        pthread_join(threads[i], NULL);
    }

    pthread_mutex_destroy(&queue_mutex);
    pthread_cond_destroy(&cond_var);
    free(t_args);
    printf("Execução finalizada.\n");
}