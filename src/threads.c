#include "threads.h"

pthread_mutex_t queue_mutex;
pthread_cond_t cond_var;
int current_core = 0;
bool running_core[NUM_CORES];
bool has_process[NUM_CORES];
bool more_process[NUM_CORES];
int total_running_cores = NUM_CORES;
cache *cache_table;

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
    fprintf(file, "Quantum remaining: %hd/ Base address: %hd/ Memory limit: %hd\n\n",
    proc->pcb->quantum_remaining,proc->pcb->base_address,proc->pcb->limit_of_memory);

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
    fprintf(file, "Quantum remaining: %hd/ Base address: %hd/ Memory limit: %hd\n\n",
    proc->pcb->quantum_remaining,proc->pcb->base_address,proc->pcb->limit_of_memory);

    fclose(file);  
}

bool avaliable_process(queue_start *queue) {
    for (unsigned short int i = 0; i < NUM_PROGRAMS; i++)
    {
        if (!queue->initial_queue[i].pcb->is_running && !queue->initial_queue[i].pcb->is_blocked && !queue->initial_queue[i].pcb->is_terminated)
            return true;
    }
    return false;
}

process *get_process(cpu* cpu, queue_start *queue, unsigned short int core_id) {
    pthread_mutex_lock(&queue_mutex);
    for (unsigned short int i = 0; i < NUM_PROGRAMS; i++) {
        if (has_process[core_id]) {
            if (queue->initial_queue[i].pcb->is_running && queue->initial_queue[i].pcb->on_core == core_id) {
                pthread_mutex_unlock(&queue_mutex);
                return &queue->initial_queue[i];
            }
        }
        else {
            if (queue->initial_queue[i].pcb->on_core == -1) {
                queue->initial_queue[i].pcb->is_running = true;
                queue->initial_queue[i].pcb->on_core = core_id;
                update_regs(cpu, queue->initial_queue[i].pcb, core_id);
                printf("Core %hd iniciou o processo id: %hd\n", core_id, queue->initial_queue[i].pcb->process_id);
                has_process[core_id] = true;
                log_start(&queue->initial_queue[i]);
                pthread_mutex_unlock(&queue_mutex);
                return &queue->initial_queue[i];
            }
            else if (queue->initial_queue[i].pcb->is_blocked && !avaliable_process(queue)) {
                queue->initial_queue[i].pcb = search_cache(cache_table, queue->initial_queue[i].pcb->process_id);
                queue->initial_queue[i].pcb->is_running = true;
                queue->initial_queue[i].pcb->on_core = core_id;
                queue->initial_queue[i].pcb->is_blocked = false;
                update_regs(cpu, queue->initial_queue[i].pcb, core_id);
                printf("Core %hd reativou o processo id: %hd\n", core_id, queue->initial_queue[i].pcb->process_id);
                has_process[core_id] = true;
                pthread_mutex_unlock(&queue_mutex);
                return &queue->initial_queue[i];
            }
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

        process *proc = get_process(t_args->cpu, t_args->queue_start, t_args->core_id);

        pthread_mutex_lock(&queue_mutex);
        if (proc == NULL) {
            printf("Core %hd finalizado.\n", t_args->core_id);
            running_core[t_args->core_id] = false;
            total_running_cores--;

            // Se não houver mais cores rodando, sinaliza fim do sistema
            if (total_running_cores == 0) {
                pthread_cond_broadcast(&cond_var);  // Acorda qualquer thread esperando e permite finalização
                pthread_mutex_unlock(&queue_mutex);
                break;
            }

            // Se ainda houver cores ativos, escolhe o próximo
            do {
                current_core = (current_core + 1) % NUM_CORES;
            } while (!running_core[current_core]);

            pthread_cond_broadcast(&cond_var);  // Notifica os outros threads sobre a mudança
            pthread_mutex_unlock(&queue_mutex);
            break;
        }
        pthread_mutex_unlock(&queue_mutex);

        if (proc && !proc->pcb->is_terminated && !quantum_over(proc)) {
            init_pipeline(t_args->cpu, t_args->memory_ram, proc, t_args->core_id);
        }

        pthread_mutex_lock(&queue_mutex);
        if (proc && quantum_over(proc)) {
            printf("Core %hd bloqueou o processo id: %hd, quantum zerado\n", t_args->core_id, proc->pcb->process_id);
            has_process[t_args->core_id] = false;
            proc->pcb->state_of_process = BLOCK;
            log_block(proc);
            proc->pcb->is_running = false;
            proc->pcb->is_blocked = true;
            proc->pcb->is_terminated = false;
            add_cache(&cache_table, proc->pcb->process_id, proc->pcb);
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
    }

    pthread_exit(NULL);
}

void init_threads(cpu *cpu, ram *memory_ram, queue_start *queue_start) {
    int cores_ativos;
    if (NUM_PROGRAMS < NUM_CORES)
        cores_ativos = NUM_PROGRAMS;
    else
        cores_ativos = NUM_CORES;
    pthread_t threads[cores_ativos];
    thread_args *t_args = malloc(sizeof(thread_args) * cores_ativos);
    init_cache(&cache_table);

    pthread_mutex_init(&queue_mutex, NULL);
    pthread_cond_init(&cond_var, NULL);

    for (int i = 0; i < cores_ativos; i++) {
        t_args[i].core_id = i;
        t_args[i].cpu = cpu;
        t_args[i].memory_ram = memory_ram;
        t_args[i].queue_start = queue_start;
        running_core[i] = true;
        more_process[i] = true;
        has_process[i] = false;

        pthread_create(&threads[i], NULL, core_function, &t_args[i]);
    }

    for (int i = 0; i < cores_ativos; i++) {
        pthread_join(threads[i], NULL);
    }

    pthread_mutex_destroy(&queue_mutex);
    pthread_cond_destroy(&cond_var);

    free(t_args);
    empty_cache(&cache_table);
    printf("Execução finalizada.\n");
}