#ifndef LOGS_H
#define LOGS_H

#include "../software/processes/queue_process.h"

void init_logs();

void init_logs_start_file();
void init_logs_block_file();
void init_logs_end_file();

void write_logs_start_file(process* proc);
void write_logs_block_file(process* proc);
void write_logs_end_file(process* proc);

#endif