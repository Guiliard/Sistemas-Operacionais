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

void add_cache2(cache2 **cache_table, const char* instruction, unsigned short int result) {
    cache2 *entry = NULL;
    HASH_FIND_STR(*cache_table, instruction, entry);

    if (entry == NULL) {
        cache2 *entry = malloc(sizeof(cache2));
        entry->instruction = strdup(instruction);
        entry->result = result;
        HASH_ADD_KEYPTR(hh, *cache_table, entry->instruction, strlen(entry->instruction), entry);
    }
}

bool search_cache2(cache2 *cache_table, const char *instruction) {
    cache2 *entry;
    HASH_FIND_STR(cache_table, instruction, entry);
    return entry != NULL;
}

unsigned short int get_result_cache(cache2 *cache_table, const char* instruction) {
    cache2 *entry;
    HASH_FIND_STR(cache_table, instruction, entry);
    return entry->result;
}

void remove_cache2(cache2 **cache_table, char* instruction) {
    cache2 *entry;
    HASH_FIND_STR(*cache_table, instruction, entry);
    if (entry) {
        HASH_DEL(*cache_table, entry);
        free(entry->instruction); // Liberar memória alocada para a string
        free(entry);
    }
}

void empty_cache2(cache2 **cache_table) {
    cache2 *entry, *tmp;
    HASH_ITER(hh, *cache_table, entry, tmp) {
        HASH_DEL(*cache_table, entry);
        free(entry->instruction);
        free(entry);
    }
}

void print_cache(cache2 *cache_table) {
    cache2 *entry;
    for (entry = cache_table; entry != NULL; entry = entry->hh.next) {
        printf("Instrucao: %s, Resultado: %d\n", entry->instruction, entry->result);
    }
}