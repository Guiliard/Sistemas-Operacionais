#include "threads.h"

pthread_mutex_t queue_mutex;
pthread_cond_t cond_var;
int current_core = 0;
bool running_core[NUM_CORES];
bool has_process[NUM_CORES];
bool more_process[NUM_CORES];
int total_running_cores = NUM_CORES;
cache *cache_table;

bool avaliable_process(process* process_queue) {
    for (unsigned short int i = 0; i < NUM_PROGRAMS; i++)
    {
        if (!process_queue[i].pcb->is_running && !process_queue[i].pcb->is_blocked && !process_queue[i].pcb->is_terminated)
            return true;
    }
    return false;
}

process *get_process(cpu* cpu, process* process_queue, unsigned short int core_id) {
    pthread_mutex_lock(&queue_mutex);
    for (unsigned short int i = 0; i < NUM_PROGRAMS; i++) {
        if (has_process[core_id]) {
            if (process_queue[i].pcb->is_running && process_queue[i].pcb->on_core == core_id) {
                pthread_mutex_unlock(&queue_mutex);
                return &process_queue[i];
            }
        }
        else {
            if (process_queue[i].pcb->on_core == -1) {
                process_queue[i].pcb->is_running = true;
                process_queue[i].pcb->on_core = core_id;
                update_regs(cpu, process_queue[i].pcb, core_id);
                printf("Core %hd iniciou o processo id: %hd\n", core_id, process_queue[i].pcb->process_id);
                has_process[core_id] = true;
                write_logs_start_file(&process_queue[i]);
                pthread_mutex_unlock(&queue_mutex);
                return &process_queue[i];
            }
            else if (process_queue[i].pcb->is_blocked && !avaliable_process(process_queue)) {
                process_queue[i].pcb = search_cache_process(cache_table, process_queue[i].pcb->process_id);
                process_queue[i].pcb->is_running = true;
                process_queue[i].pcb->on_core = core_id;
                process_queue[i].pcb->is_blocked = false;
                update_regs(cpu, process_queue[i].pcb, core_id);
                printf("Core %hd reativou o processo id: %hd\n", core_id, process_queue[i].pcb->process_id);
                has_process[core_id] = true;
                pthread_mutex_unlock(&queue_mutex);
                return &process_queue[i];
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

        process *proc = get_process(t_args->cpu, t_args->process_queue, t_args->core_id);

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
            init_pipeline(t_args->cpu, t_args->memory_ram, proc, t_args->core_id, cache_table);
        }

        pthread_mutex_lock(&queue_mutex);
        if (proc && quantum_over(proc)) {
            printf("Core %hd bloqueou o processo id: %hd, quantum zerado\n", t_args->core_id, proc->pcb->process_id);
            has_process[t_args->core_id] = false;
            proc->pcb->state_of_process = BLOCK;
            proc->pcb->is_running = false;
            proc->pcb->is_blocked = true;
            proc->pcb->is_terminated = false;
            write_logs_block_file(proc);
            proc->pcb->quantum_remaining = proc->pcb->total_quantum;
            add_cache_process(cache_table, proc->pcb->process_id, proc->pcb);
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

void init_threads(cpu *cpu, ram *memory_ram, process* process_queue) {
    int cores_ativos;
    
    cache_table = malloc(sizeof(cache));
    init_cache(cache_table);

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
        t_args[i].process_queue = process_queue;
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
    empty_cache(cache_table);
    printf("Execução finalizada.\n");
}