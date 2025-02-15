#include "scheduler.h"

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

bool quantum_over(process* process) {
    if (process->pcb->quantum_remaining <= 0) {
        return true;
    }
    return false;
}