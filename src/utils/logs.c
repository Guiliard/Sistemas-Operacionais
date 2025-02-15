#include "logs.h"

void init_logs() {
    init_logs_start_file();
    init_logs_block_file();
    init_logs_end_file();
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