#include "queues.h"

void init_queue_start(queue_start* initial_queue) {

    initial_queue->initial_queue = malloc(NUM_PROGRAMS * sizeof(process));

    if (initial_queue->initial_queue == NULL) {
        printf("Error: memory allocation failed in queue_start\n");
        exit(1);
    }

    for (unsigned short int i = 0; i < NUM_PROGRAMS; i++) {
        initial_queue->initial_queue[i].program = NULL;
        initial_queue->initial_queue[i].pcb = init_pcb();  
    }
}

void init_queue_end(queue_end* final_queue) {
    final_queue->final_queue = malloc(NUM_PROGRAMS * sizeof(process*));
    if (final_queue->final_queue == NULL) {
        printf("Error: memory allocation failed in queue_end\n");
        exit(1);
    }

    for (unsigned short int i = 0; i < NUM_PROGRAMS; i++) {
        final_queue->final_queue[i].program = NULL;
    }
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
    fprintf(file, "Result of process: %s\n\n",
    proc->pcb->result_of_process);

    fclose(file);  
}

void populate_queue_start(queue_start* initial_queue, ram* memory_ram) {
    
    char* programs_on_ram = strdup(memory_ram->vector); 
    
    if (!programs_on_ram) {
        printf("Memory allocation failed for programs on ram\n");
        exit(1);
    }

    char* only_process = strtok(programs_on_ram, "#"); 
    unsigned short int i = 0;

    while (only_process != NULL && i < NUM_PROGRAMS) {
        
        while (only_process[0] == ' ' || only_process[0] == '\n' || only_process[0] == '\t') {
            only_process++;
        }

        initial_queue->initial_queue[i].program = malloc(strlen(only_process) + 1);

        if (initial_queue->initial_queue[i].program == NULL) {
            printf("Memory allocation failed for program %d\n", i);
            exit(1);
        }
        
        strcpy(initial_queue->initial_queue[i].program, only_process);

        initial_queue->initial_queue[i].pcb->process_id = i;
        initial_queue->initial_queue[i].pcb->priority = i;
        initial_queue->initial_queue[i].pcb->base_address = (unsigned long)(only_process - programs_on_ram);

        i++;
        only_process = strtok(NULL, "#"); 
    }

    free(programs_on_ram); 
}

void add_resource_to_pcb(process_control_block *pcb, char *memory_adress) {
    if (pcb->resource_name == NULL) {
        size_t length = strlen(memory_adress) + 3; 
        pcb->resource_name = (char *)malloc(length);

        if (pcb->resource_name == NULL) {
            printf("Error: memory allocation failed in bank of register used\n");
            exit(1);
        }

        snprintf(pcb->resource_name, length, "%s, ", memory_adress);
    } else {

        size_t bank_length = strlen(pcb->resource_name);
        char *pattern = (char *)malloc(strlen(memory_adress) + 3);
        snprintf(pattern, strlen(memory_adress) + 3, "%s, ", memory_adress);

        if (strstr(pcb->resource_name, pattern) == NULL) {
            size_t new_length = bank_length + strlen(memory_adress) + 3; 
            char *new_memory = (char *)realloc(pcb->resource_name, new_length);

            if (new_memory == NULL) {
                printf("Error: memory allocation failed in resource name\n");
                free(pattern);
                exit(1);
            }

            pcb->resource_name = new_memory;
            strcat(pcb->resource_name, memory_adress);
            strcat(pcb->resource_name, ", ");
        }

        free(pattern);
    }
}

void check_resources_on_queue_start(queue_start* initial_queue) {
    char* line;
    unsigned short int num_line = 0;
    unsigned short int num;
    char register_name1[10];
    char memory_address[10];
    type_of_instruction type;

    for (unsigned short int i = 0; i < NUM_PROGRAMS; i++) {

        num = count_lines(initial_queue->initial_queue[i].program);

        while (num_line < num) {
            line = get_line_of_program(initial_queue->initial_queue[i].program, num_line);
            type = verify_instruction(line, num_line);

            if (type == STORE) {
                sscanf(line, "STORE %9s %9s", register_name1, memory_address);

                trim(memory_address);

                add_resource_to_pcb(initial_queue->initial_queue[i].pcb, memory_address);
            }

            num_line++;
        }   
        num = 0;
        num_line = 0;
    }
}

void add_process_to_queue_end(queue_end* final_queue, process* process) {
    process->pcb->state_of_process = READY;
    process->pcb->is_terminated = true;
    
    for (unsigned short int i = 0; i < NUM_PROGRAMS; i++) {
        if (final_queue->final_queue[i].program == NULL) {
            final_queue->final_queue[i] = *process;
            break;
        }
    }
}

void organize_process_of_queue_start(queue_start* initial_queue, unsigned short int process_id) {

    for (unsigned short int i = process_id; i < NUM_PROGRAMS - 1; i++) {
        initial_queue->initial_queue[i] = initial_queue->initial_queue[i + 1];
    }
    
    if (initial_queue->initial_queue[NUM_PROGRAMS - 1].program != NULL) {
        initial_queue->initial_queue[NUM_PROGRAMS - 1].program = NULL;
        initial_queue->initial_queue[NUM_PROGRAMS - 1].pcb = NULL;
    }
}