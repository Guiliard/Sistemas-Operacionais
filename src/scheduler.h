#ifndef SCHEDULER_H
#define SCHEDULER_H

#include "architecture.h"

int compare_priority(const void* a, const void* b);
bool quantum_over(process* process);

#endif 