#include "threads.h"

pthread_mutex_t queue_mutex;
pthread_cond_t cond_var;
int current_core = 0;
unsigned short int processos_pendentes = NUM_PROGRAMS;

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

process *get_process(queue_start *queue) {
    pthread_mutex_lock(&queue_mutex);
    printf("ohyeah");
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
        while (t_args->core_id != current_core) {
            pthread_cond_wait(&cond_var, &queue_mutex);
        }

        process *proc = t_args->assigned_process;
        if (proc && !proc->pcb->is_terminated) {
            printf("Core %d executando processo %p\n", t_args->core_id, (void *)proc);
            init_pipeline(t_args->cpu, t_args->memory_ram, proc->program, proc->pcb, t_args->core_id);

            if (proc->pcb->is_terminated) {
                printf("Core %d terminou o processo %p\n", t_args->core_id, (void *)proc);
                processos_pendentes--;
                t_args->assigned_process = NULL;
            }
        }

        // Verifica se há novos processos disponíveis na fila
        if (!t_args->assigned_process) {
            process *new_proc = get_process(t_args->queue_start);
            if (new_proc) {
                t_args->assigned_process = new_proc;
                printf("Core %d recebeu novo processo %p\n", t_args->core_id, (void *)new_proc);
            } else if (processos_pendentes == 0) {
                printf("Core %d não tem mais processos para executar. Encerrando.\n", t_args->core_id);
                pthread_mutex_unlock(&queue_mutex);
                break;  // Sai do loop e finaliza a thread
            }
        }

        current_core = (current_core + 1) % NUM_CORES;
        pthread_cond_broadcast(&cond_var);
        pthread_mutex_unlock(&queue_mutex);
    }

    current_core = (current_core + 1) % NUM_CORES;
    pthread_cond_broadcast(&cond_var);
    pthread_mutex_unlock(&queue_mutex);
    printf("Core %d finalizou a execução.\n", t_args->core_id);
    pthread_exit(NULL);
}


void init_threads(cpu *cpu, ram *memory_ram, queue_start *queue_start, queue_end *queue_end) {
    int cores_ativos = NUM_PROGRAMS < NUM_CORES ? NUM_PROGRAMS : NUM_CORES;
    pthread_t threads[cores_ativos];
    thread_args *t_args = malloc(sizeof(thread_args) * cores_ativos);

    pthread_mutex_init(&queue_mutex, NULL);
    pthread_cond_init(&cond_var, NULL);
    processos_pendentes = NUM_PROGRAMS;

    for (int i = 0; i < cores_ativos; i++) {
        t_args[i].core_id = i;
        t_args[i].cpu = cpu;
        t_args[i].memory_ram = memory_ram;
        t_args[i].queue_start = queue_start;
        queue_end = queue_end;
        t_args[i].assigned_process = get_process(queue_start);

        if (t_args[i].assigned_process == NULL) {
            printf("Nenhum processo disponível para o core %d\n", i);
        } else {
            printf("Core %d iniciou com processo %p\n", i, (void *)t_args[i].assigned_process);
        }

        pthread_create(&threads[i], NULL, core_function, &t_args[i]);
    }

    for (int i = 0; i < cores_ativos; i++) {
        pthread_join(threads[i], NULL);
    }

    pthread_mutex_destroy(&queue_mutex);
    pthread_cond_destroy(&cond_var);
    free(t_args);
    printf("Execução finalizada.\n");
}


