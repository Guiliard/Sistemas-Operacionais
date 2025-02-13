#ifndef SCHEDULER_H
#define SCHEDULER_H

#include "../processes/queue_process.h"

int compare_priority(const void* a, const void* b);
bool quantum_over(process* process);

#endif 