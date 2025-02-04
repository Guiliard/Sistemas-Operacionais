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
