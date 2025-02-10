#include "cache.h"

void init_cache(cache **cache_table) {
    *cache_table = NULL;
}

void add_cache(cache **cache_table, unsigned short int process_id, process_control_block *process_pcb) {
    cache *item = NULL;

    HASH_FIND(hh, *cache_table, &process_id, sizeof(unsigned short int), item);

    if (item == NULL) {
        item = (cache *)malloc(sizeof(cache));
        if (item == NULL) {
            printf("Error: memory allocation failed in add to cache\n");
            exit(1);
        }
        item->process_id = process_id;
        item->process_pcb = process_pcb; // Armazena o ponteiro para PCB
        HASH_ADD(hh, *cache_table, process_id, sizeof(unsigned short int), item);
    } else {
        item->process_pcb = process_pcb; // Atualiza o valor caso já exista
    }
}

process_control_block *search_cache(cache *cache_table, unsigned short int process_id) {
    cache *item = NULL;

    HASH_FIND(hh, cache_table, &process_id, sizeof(unsigned short int), item);

    return item ? item->process_pcb : NULL;
}

void remove_cache(cache **cache_table, unsigned short int process_id) {
    cache *item = NULL;

    HASH_FIND(hh, *cache_table, &process_id, sizeof(unsigned short int), item);
    if (item != NULL) {
        HASH_DEL(*cache_table, item);
        free(item);
    } else {
        printf("Error: process_id not found in cache\n");
        exit(1);
    }
}

void empty_cache(cache **cache_table) {
    cache *item, *tmp;
    
    HASH_ITER(hh, *cache_table, item, tmp) {
        HASH_DEL(*cache_table, item);
        free(item);
    }
}

void init_cache2(cache2 **cache_table) {
    *cache_table = NULL;
}

void add_cache2(cache2 **cache_table, char *instruction, unsigned short int result) {
    cache2 *item = NULL;

    HASH_FIND_STR(*cache_table, instruction, item);

    if (item == NULL) {
        item = (cache2 *)malloc(sizeof(cache2));
        if (item == NULL) {
            printf("Error: memory allocation failed in add to cache2\n");
            exit(1);
        }
        item->instruction = strdup(instruction); // Copia a string para evitar problemas de ponteiro
        item->result = result;
        HASH_ADD_STR(*cache_table, instruction, item);
    } else {
        item->result = result; // Atualiza o valor caso já exista
    }
}

bool search_cache2(cache2 *cache_table, char *instruction) {
    cache2 *item = NULL;
    HASH_FIND_STR(cache_table, instruction, item);
    
    if (item) {
        return true;
    } else {
        return false; // Retorna 0 se não encontrar
    }
}

unsigned short int get_result_cache(cache2 *cache_table, char *instruction) {
    cache2 *item = NULL;
    HASH_FIND_STR(cache_table, instruction, item);
    
    return item->result;
}

void remove_cache2(cache2 **cache_table, char *instruction) {
    cache2 *item = NULL;

    HASH_FIND_STR(*cache_table, instruction, item);
    if (item != NULL) {
        HASH_DEL(*cache_table, item);
        free(item->instruction); // Libera a string alocada
        free(item);
    } else {
        printf("Error: instruction not found in cache2\n");
        exit(1);
    }
}

void empty_cache2(cache2 **cache_table) {
    cache2 *item, *tmp;
    
    HASH_ITER(hh, *cache_table, item, tmp) {
        HASH_DEL(*cache_table, item);
        free(item->instruction); // Libera a string alocada
        free(item);
    }
}