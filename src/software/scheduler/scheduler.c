#include "scheduler.h"

void shuffle(unsigned short int* arr) {
    for (int i = 0; i < NUM_PROGRAMS; i++) {
        int j = rand() % NUM_PROGRAMS;
        int temp = arr[i];
        arr[i] = arr[j];
        arr[j] = temp;
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

void fifo(process* process_queue) {
    for (unsigned short int i = 0; i < NUM_PROGRAMS; i++) {
        process_queue[i].pcb->quantum_remaining = (rand() % 6) + 5; 
        process_queue[i].pcb->total_quantum = process_queue[i].pcb->quantum_remaining;
    }
}

void round_robin(process* process_queue) {
    for (unsigned short int i = 0; i < NUM_PROGRAMS; i++) {
        process_queue[i].pcb->quantum_remaining = 10; 
        process_queue[i].pcb->total_quantum = process_queue[i].pcb->quantum_remaining;
    }
}

void priority(process* process_queue) {
    unsigned short int prioridades[NUM_PROGRAMS];

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

    fifo(process_queue);
}

bool quantum_over(process* process) {
    if (process->pcb->quantum_remaining <= 0) {
        return true;
    }
    return false;
}