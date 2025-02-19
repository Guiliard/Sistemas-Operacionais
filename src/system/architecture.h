#ifndef ARCHITECTURE_H
#define ARCHITECTURE_H

#include "../hardware/memory/cache.h"
#include "../software/pipeline/pipeline.h"
#include "../utils/libs.h"
#include "../hardware/cpu/cpu.h"
#include "../software/control/control_unit.h"
#include "../software/processes/queue_process.h"
#include "../software/processes/pcb.h"
#include "../utils/logs.h"
#include "../software/scheduler/scheduler.h"

void init_architecture(cpu* cpu, ram* memory_ram, process* process_queue);
void load_program_on_ram(ram* memory, char* program);
void check_instructions_on_ram(ram* memory_ram);
void init_pipeline(cpu* cpu, ram* memory_ram, process* process, unsigned short int core_number, cache* cache_table, type_policy policy_type, unsigned short int* time_quantum);
void update_regs(cpu* cpu, process_control_block* pcb, unsigned short int core_number);
void free_architecture(cpu* cpu, ram* memory_ram, process* process_queue);

#endif