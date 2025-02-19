#ifndef SCHEDULER_H
#define SCHEDULER_H

#include "../../utils/libs.h"
#include "../processes/pcb.h"

typedef enum type_scheduler {
    FIFO,
    ROUND_ROBIN,
    PRIORITY
} type_scheduler;

typedef struct process {
    char *program;
    process_control_block *pcb;
} process;

void scheduler(process* process_queue, type_scheduler scheduler_type);
void fifo(process* process_queue);
void round_robin(process* process_queue);
void priority(process* process_queue);

void similiarity(process* process_queue);
void sort_by_similarity(process* process_queue, similarity_score* score);

void shuffle(unsigned short int* arr);

bool quantum_over(process* process);

#endif 