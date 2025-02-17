#ifndef LOGS_H
#define LOGS_H

#include "../software/processes/queue_process.h"
#include "../software/scheduler/scheduler.h"
#include "../hardware/memory/cache.h"

void init_logs();

void init_logs_start_file();
void init_logs_block_file();
void init_logs_end_file();
void init_logs_system_file();

void write_logs_start_file(process* proc);
void write_logs_block_file(process* proc);
void write_logs_end_file(process* proc);
void write_logs_system_file(char* message);

char* enum_schedular_to_string(type_scheduler scheduler_type);
char* enum_cache_policy_to_string(type_policy policy_type);

#endif