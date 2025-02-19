#ifndef CACHE_H
#define CACHE_H

#include "../../utils/uthash.h"
#include "../../utils/libs.h"
#include "../../software/processes/pcb.h"
#include "../../hardware/cpu/cpu.h"

typedef struct hash_instruction {
    char* instruction; // Chave (instruction)
    unsigned short int result; // Valor (resultado da instrução)
    unsigned int frequency; // Frequência de uso
    UT_hash_handle hh; 
} hash_instruction;

typedef struct hash_process {
    unsigned short int process_id; // Chave (process_id do PCB)
    process_control_block *process_pcb; // Valor (ponteiro para a estrutura PCB)
    UT_hash_handle hh;  
} hash_process;

typedef struct cache {
    hash_process *process_table;
    hash_instruction *instruction_table;
} cache;

typedef struct instruction_cache_item {
    bool is_cached;
    unsigned short int reg_index;
    unsigned short int result;
} instruction_cache_item;

typedef enum type_policy {
    FIFO_POLICY,
    LEAST_RECENTLY_USED,
    RANDOM_REPLACEMENT
} type_policy;

void init_cache(cache *cache_table);

void policy(cache *cache_table, type_policy policy_type);
void fifo_policy(cache *cache_table);
void lru_policy(cache *cache_table);
void random_policy(cache *cache_table);

void add_cache_process(cache *cache_table, unsigned short int process_id, process_control_block *process_pcb);
void add_cache_instruction(cache *cache_table, const char* instruction, unsigned short int result, type_policy policy_type);

process_control_block *search_cache_process(cache *cache_table, unsigned short int process_id);
bool search_cache_instruction(cache *cache_table, const char *instruction);

void empty_cache(cache *cache_table);

unsigned short int get_result_cache_instruction(cache *cache_table, char* instruction);

void print_cache_instruction(cache *cache_table);

instruction_cache_item* init_instruction_cache_item();

#endif