#include "scheduler.h"

void shuffle(unsigned short int* array_priority) {
    for (int i = 0; i < NUM_PROGRAMS; i++) {
        int j = rand() % NUM_PROGRAMS;
        int temp = array_priority[i];
        array_priority[i] = array_priority[j];
        array_priority[j] = temp;
    }
}

void sort_by_similarity(process* process_queue, similarity_score* score) {
    for (unsigned short int i = 0; i < NUM_PROGRAMS - 1; i++) {
        for (unsigned short int j = i + 1; j < NUM_PROGRAMS; j++) {
            if (score[i].score_arithmetic < score[j].score_arithmetic ||
                (score[i].score_arithmetic == score[j].score_arithmetic && score[i].score_control < score[j].score_control) ||
                (score[i].score_arithmetic == score[j].score_arithmetic && score[i].score_control == score[j].score_control && score[i].score_memory < score[j].score_memory)) {
                process temp = process_queue[i];
                process_queue[i] = process_queue[j];
                process_queue[j] = temp;

                similarity_score temp_score = score[i];
                score[i] = score[j];
                score[j] = temp_score;
            }
        }
    }
}

void scheduler(process* process_queue, type_scheduler scheduler_type) {
    switch (scheduler_type) {
        case FIFO:
            fifo(process_queue);
            break;
        case ROUND_ROBIN:
            round_robin(process_queue);
            break;
        case PRIORITY:
            priority(process_queue);
            break;
    }
}

void similiarity(process* process_queue) {
    similarity_score* score = malloc(NUM_PROGRAMS* sizeof(similarity_score));
    unsigned short int* indexs = malloc(NUM_PROGRAMS * sizeof(unsigned short int));
    char* line;
    unsigned short int num_line = 0;

    for (unsigned short int i = 0; i < NUM_PROGRAMS; i++) {
        score[i] = *init_similarity_score(); 
        indexs[i] = i;
        num_line = count_lines(process_queue[i].program);

        for (unsigned short int j = 0; j < num_line; j++) {
            line = get_line_of_program(process_queue[i].program, j);
            get_similarity_score(&score[i], line);
            free(line);
        }
    }

    sort_by_similarity(process_queue, score);

    free(score);
}

void fifo(process* process_queue) {

    similiarity(process_queue);

    for (unsigned short int i = 0; i < NUM_PROGRAMS; i++) {
        process_queue[i].pcb->quantum_remaining = (rand() % 6) + 5; 
        process_queue[i].pcb->total_quantum = process_queue[i].pcb->quantum_remaining;
    }
}

void round_robin(process* process_queue) {

    similiarity(process_queue);

    for (unsigned short int i = 0; i < NUM_PROGRAMS; i++) {
        process_queue[i].pcb->quantum_remaining = 10; 
        process_queue[i].pcb->total_quantum = process_queue[i].pcb->quantum_remaining;
    }
}

void priority(process* process_queue) {
    unsigned short int* prioridades =  malloc(NUM_PROGRAMS * sizeof(unsigned short int));

    for (unsigned short int i = 0; i < NUM_PROGRAMS; i++) {
        prioridades[i] = i;
    }

    shuffle(prioridades);

    for (unsigned short int i = 0; i < NUM_PROGRAMS; i++) {
        process_queue[i].pcb->priority = prioridades[i];
    }

    for (unsigned short int i = 0; i < NUM_PROGRAMS - 1; i++) {
        for (unsigned short int j = i + 1; j < NUM_PROGRAMS; j++) {
            if (process_queue[i].pcb->priority > process_queue[j].pcb->priority) {
                process temp = process_queue[i];
                process_queue[i] = process_queue[j];
                process_queue[j] = temp;
            }
        }
    }

    for (unsigned short int i = 0; i < NUM_PROGRAMS; i++) {
        process_queue[i].pcb->quantum_remaining = (rand() % 6) + 5; 
        process_queue[i].pcb->total_quantum = process_queue[i].pcb->quantum_remaining;
    }
}

bool quantum_over(process* process) {
    if (process->pcb->quantum_remaining <= 0) {
        return true;
    }
    return false;
}