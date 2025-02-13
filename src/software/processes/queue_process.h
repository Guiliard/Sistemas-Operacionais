#ifndef QUEUE_PROCESS_H
#define QUEUE_PROCESS_H

#define NUM_PROGRAMS 4

#include "../../utils/libs.h"
#include "../../hardware/memory/ram.h"
#include "pcb.h"
#include "../../utils/reader.h"
#include "../control/interpreter.h"

typedef struct process {
    char *program;
    process_control_block *pcb;
} process;

void init_process_queue(process* process_queue);
void populate_process_queue(process* process_queue, ram* memory_ram);
void check_resources_on_process_queue(process* process_queue);

#endif