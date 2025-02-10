#ifndef CACHE_H
#define CACHE_H

#include "uthash.h"
#include "libs.h"
#include "pcb.h"

typedef struct cache {
    unsigned short int process_id; // Chave (process_id do PCB)
    process_control_block *process_pcb; // Valor (ponteiro para a estrutura PCB)
    UT_hash_handle hh;
} cache;

typedef struct cache2 {
    char* instruction; // Chave (process_id do PCB)
    unsigned short int result; // Valor (ponteiro para a estrutura PCB)
    UT_hash_handle hh;
} cache2;

void init_cache(cache **cache_table);
void add_cache(cache **cache_table, unsigned short int process_id, process_control_block *process_pcb);
process_control_block *search_cache(cache *cache_table, unsigned short int process_id);
void remove_cache(cache **cache_table, unsigned short int process_id);
void empty_cache(cache **cache_table);

void init_cache2(cache2 **cache_table);
void add_cache2(cache2 **cache_table, char* instruction, unsigned short int result);
bool search_cache2(cache2 *cache_table, char *instruction);
unsigned short int get_result_cache(cache2 *cache_table, char* instruction);
void remove_cache2(cache2 **cache_table, char* instruction);
void empty_cache2(cache2 **cache_table);

#endif