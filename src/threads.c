#include "threads.h"

pthread_mutex_t memory_mutex;
pthread_mutex_t queue_mutex;

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

process* dequeue_process(queue_start* queue) {
    if (queue->size == 0) {
        return NULL;
    }
    process* proc = &queue->initial_queue[0];
    for (int i = 1; i < queue->size; i++) {
        queue->initial_queue[i - 1] = queue->initial_queue[i];
    }
    queue->size--;
    return proc;
}

bool queue_is_empty(queue_start* queue) {
    if (queue->size == 0)
        return true;
    else
        return false;
}

void* core_function(void* args) {
    thread_args* t_args = (thread_args*)args;
    process* current_process = NULL;

    while (1) {
        pthread_mutex_lock(&queue_mutex);
        if (queue_is_empty(t_args->queue_start)) {
            pthread_mutex_unlock(&queue_mutex);
            break;
        }
        current_process = dequeue_process(t_args->queue_start);
        pthread_mutex_unlock(&queue_mutex);

        printf("Core %d: Iniciando execucao do programa:\n%s\n", t_args->core_id, current_process->program);

        current_process->pcb->core_number = t_args->core_id;
        current_process->pcb->in_p->num_instruction = 0;

        while (!current_process->pcb->is_terminated) {
            pthread_mutex_lock(&memory_mutex);
            init_pipeline(t_args->cpu, t_args->memory_ram, current_process->program, current_process->pcb);
            pthread_mutex_unlock(&memory_mutex);
        }

        add_process_to_queue_end(t_args->queue_end, current_process);
        log_end(current_process);
        printf("Core %d: Programa finalizado.\n", t_args->core_id);
    }

    printf("Core %d: Nenhum programa restante. Encerrando.\n", t_args->core_id);
    pthread_exit(NULL);
}

void init_threads(cpu* cpu, ram* memory_ram, queue_start* queue_start, queue_end* queue_end) {
    pthread_t threads[NUM_CORES];
    thread_args t_args[NUM_CORES];

    pthread_mutex_init(&queue_mutex, NULL);
    pthread_mutex_init(&memory_mutex, NULL);

    for (int i = 0; i < NUM_CORES; i++) {
        t_args[i].cpu = cpu;
        t_args[i].memory_ram = memory_ram;
        t_args[i].queue_start = queue_start;
        t_args[i].queue_end = queue_end;
        t_args[i].core_id = i;

        if (pthread_create(&threads[i], NULL, core_function, &t_args[i]) != 0) {
            perror("Error: Fail on creating thread");
            exit(1);
        }
    }

    for (int i = 0; i < NUM_CORES; i++) {
        pthread_join(threads[i], NULL);
    }

    pthread_mutex_destroy(&queue_mutex);
    pthread_mutex_destroy(&memory_mutex);
}





