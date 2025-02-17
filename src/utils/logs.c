#include "logs.h"

void init_logs() {
    init_logs_start_file();
    init_logs_block_file();
    init_logs_end_file();
    init_logs_system_file();
}

void init_logs_start_file() {
    FILE* file = fopen("output/start.txt", "w");  
    if (file == NULL) {
        perror("Error: Cannot create output/start.txt");
        exit(1);  
    }
    fprintf(file, "Queue of programs to be executed.");
    fclose(file);
}

void init_logs_block_file() {
    FILE* file = fopen("output/block.txt", "w");  
    if (file == NULL) {
        perror("Error: Cannot create output/block.txt");
        exit(1);  
    }
    fprintf(file, "Queue of blocked programs.");
    fclose(file);
}

void init_logs_end_file() {
    FILE* file = fopen("output/end.txt", "w");  
    if (file == NULL) {
        perror("Error: Cannot create output/end.txt");
        exit(1);  
    }
    fprintf(file, "Queue of terminated programs.");
    fclose(file);
}

void init_logs_system_file() {
    FILE* file = fopen("output/system.txt", "w");  
    if (file == NULL) {
        perror("Error: Cannot create output/system.txt");
        exit(1);  
    }
    fprintf(file, "Logs of Operating System.\n\n");
    fclose(file);
}

void write_logs_start_file(process* proc) {
    FILE* file = fopen("output/start.txt", "a");  
    if (file == NULL) {
        perror("Error: Cannot open output/start.txt");
        return;
    }

    fprintf(file, "\n\n------------------PROGRAM %hd------------------\n", proc->pcb->process_id);
    fprintf(file, "%s\n", proc->program);
    
    write_pcb_to_file(file, proc->pcb);

    fclose(file);  
}

void write_logs_block_file(process* proc) {
    FILE* file = fopen("output/block.txt", "a");  
    if (file == NULL) {
        perror("Error: Cannot open output/block.txt");
        return;
    }
    fprintf(file, "\n\n------------------PROGRAM %hd------------------\n", proc->pcb->process_id);
    fprintf(file, "%s\n", proc->program);
    
    write_pcb_to_file(file, proc->pcb);

    fclose(file);  
}

void write_logs_end_file(process* proc) {
    FILE* file = fopen("output/end.txt", "a");  
    if (file == NULL) {
        perror("Error: Cannot open output/end.txt");
        return;
    }

    fprintf(file, "\n\n------------------PROGRAM %hd------------------\n", proc->pcb->process_id);
    fprintf(file, "%s\n", proc->program);
   
    write_pcb_to_file(file, proc->pcb);

    fclose(file);  
}

void write_logs_system_file(char* message) {
    FILE* file = fopen("output/system.txt", "a");  
    if (file == NULL) {
        perror("Error: Cannot open output/system.txt");
        return;
    }

    fprintf(file, "%s", message);

    fclose(file);  
}

char* enum_schedular_to_string(type_scheduler scheduler_type) {
    switch (scheduler_type) {
        case FIFO:
            return "Type of scheduler: FIFO\n";
        case ROUND_ROBIN:
            return "Type of scheduler: ROUND ROBIN\n";
        case PRIORITY:
            return "Type of scheduler: PRIORITY\n";
        default:
            return "Type of scheduler: UNKNOWN\n";
    }
}

char* enum_cache_policy_to_string(type_policy policy_type) {
    switch (policy_type) {
        case FIFO_POLICY:
            return "Type of policy cache: FIFO\n\n";
        case LEAST_RECENTLY_USED:
            return "Type of policy cache: LEAST RECENTLY USED\n\n";
        case RANDOM_REPLACEMENT:
            return "Type of policy cache: RANDOM REPLACEMENT\n\n";
        default:
            return "Type of policy cache: UNKNOWN\n\n";
    }
}
