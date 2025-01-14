#include "threads.h"

pthread_mutex_t queue_mutex;
pthread_cond_t cond_var = PTHREAD_COND_INITIALIZER;
int current_core = 0;

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
    for (int i = 0; i < NUM_PROGRAMS; i++) {
        printf("Verificando o queue[%d]...\n", i);
        if (queue[i].initial_queue != NULL) {
            printf("initial_queue está OK para queue[%d]\n", i);
            if (queue[i].initial_queue->pcb != NULL) {
                printf("PCB está OK para queue[%d]\n", i);
                if (!queue[i].initial_queue->pcb->is_running && !queue[i].initial_queue->pcb->is_terminated) {
                    queue[i].initial_queue->pcb->is_running = true;
                    return &queue->initial_queue[i];
                }
            } else {
                printf("PCB NULL na posição %d\n", i);
            }
        } else {
            printf("initial_queue NULL na posição %d\n", i);
        }
    }
    return NULL;
}


void *core_function(void *args) {
    thread_args *t_args = (thread_args *)args;
    process* proc = NULL;

    while (1) {
        pthread_mutex_lock(&queue_mutex);

        // Espera a vez do core
        while (t_args->core_id != current_core) {
            pthread_cond_wait(&cond_var, &queue_mutex);
        }

        // Pega um novo processo se necessário
        if (!proc || proc->pcb->is_terminated) {
            printf("Core %d executando processo %p\n", t_args->core_id, (void *)proc);
            proc = get_process(t_args->queue_start);
            if (!proc) {
                // Sinaliza o próximo core antes de sair
                current_core = (current_core + 1) % NUM_CORES;
                pthread_cond_broadcast(&cond_var);
                pthread_mutex_unlock(&queue_mutex);
                break;
            }
        }

        pthread_mutex_unlock(&queue_mutex);

        printf("Core %d executando processo %p\n", t_args->core_id, (void *)proc);
        printf("PCB: %p, Programa: %p\n", (void *)proc->pcb, (void *)proc->program);

        // Executa uma instrução do pipeline
        init_pipeline(t_args->cpu, t_args->memory_ram, proc->program, proc->pcb, t_args->core_id);

        pthread_mutex_lock(&queue_mutex);
        // Alterna para o próximo core
        current_core = (current_core + 1) % NUM_CORES;
        pthread_cond_broadcast(&cond_var);  // Acorda todos os cores
        pthread_mutex_unlock(&queue_mutex);
    }

    printf("Core %d finalizou a execução.\n", t_args->core_id);
    pthread_exit(NULL);
}

void init_threads(cpu* cpu, ram* memory_ram, queue_start *queue_start, queue_end *queue_end) {
    pthread_t threads[NUM_CORES];
    thread_args *t_args = malloc(sizeof(thread_args) * NUM_CORES);

    pthread_mutex_init(&queue_mutex, NULL);

    for (int i = 0; i < NUM_CORES; i++) {
        t_args[i].core_id = i;
        t_args[i].cpu = cpu;
        t_args[i].memory_ram = memory_ram;
        t_args[i].queue_start = queue_start;
        t_args[i].queue_end = queue_end;
        pthread_create(&threads[i], NULL, core_function, &t_args[i]);
    }

    for (int i = 0; i < NUM_CORES; i++) {
        pthread_join(threads[i], NULL);
    }

    pthread_mutex_destroy(&queue_mutex);
    printf("Execução finalizada.\n");
}





