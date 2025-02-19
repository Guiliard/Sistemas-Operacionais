#ifndef QUEUE_PROCESS_H
#define QUEUE_PROCESS_H

#include "../../utils/libs.h"
#include "../../hardware/memory/ram.h"
#include "../scheduler/scheduler.h"

void init_process_queue(process* process_queue);
void populate_process_queue(process* process_queue, ram* memory_ram, type_scheduler scheduler_type);

#endif