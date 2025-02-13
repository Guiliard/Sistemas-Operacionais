#include "queue_process.h"

void init_process_queue(process* process_queue) {
    
    if (process_queue == NULL) {
        printf("Error: process queue is NULL\n");
        exit(1);
    }

    for (unsigned short int i = 0; i < NUM_PROGRAMS; i++) {
        process_queue[i].program = NULL;
        process_queue[i].pcb = init_pcb();
    }
}

void populate_process_queue(process* process_queue, ram* memory_ram) {
    
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

        process_queue[i].program = malloc(strlen(only_process) + 1);

        if (process_queue[i].program == NULL) {
            printf("Memory allocation failed for program %d\n", i);
            exit(1);
        }
        
        strcpy(process_queue[i].program, only_process);

        process_queue[i].pcb->process_id = i;
        process_queue[i].pcb->priority = i;
        process_queue[i].pcb->base_address = (unsigned long)(only_process - programs_on_ram);

        i++;
        only_process = strtok(NULL, "#"); 
    }

    free(programs_on_ram); 
}

void check_resources_on_process_queue(process* process_queue) {
    char* line;
    unsigned short int num_line = 0;
    unsigned short int num;
    char register_name1[10];
    char memory_address[10];
    type_of_instruction type;

    for (unsigned short int i = 0; i < NUM_PROGRAMS; i++) {

        num = count_lines(process_queue[i].program);

        while (num_line < num) {
            line = get_line_of_program(process_queue[i].program, num_line);
            type = verify_instruction(line, num_line);

            if (type == STORE) {
                sscanf(line, "STORE %9s %9s", register_name1, memory_address);

                trim(memory_address);

                add_resource_to_pcb(process_queue[i].pcb, memory_address);
            }

            num_line++;
        }   
        num = 0;
        num_line = 0;
    }
}