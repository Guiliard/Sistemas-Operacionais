#include "cache.h"

void init_cache(cache *cache_table) {
    cache_table->process_table = NULL;
    cache_table->instruction_table = NULL;
}

void add_cache_process(cache *cache_table, unsigned short int process_id, process_control_block *process_pcb) {
    hash_process *item = NULL;

    HASH_FIND(hh, cache_table->process_table, &process_id, sizeof(unsigned short int), item);

    if (item == NULL) {
        item = (hash_process *)malloc(sizeof(hash_process));
        if (item == NULL) {
            printf("Error: memory allocation failed in add to cache\n");
            exit(1);
        }
        item->process_id = process_id;
        item->process_pcb = process_pcb;  

        HASH_ADD(hh, cache_table->process_table, process_id, sizeof(unsigned short int), item);
    } else {
        item->process_pcb = process_pcb;
    }
}

void add_cache_instruction(cache *cache_table, char *instruction, unsigned short int result) {
    hash_instruction *item = NULL;

    HASH_FIND_STR(cache_table->instruction_table, instruction, item);

    if (item == NULL) {
        item = (hash_instruction *)malloc(sizeof(hash_instruction));
        if (item == NULL) {
            printf("Error: memory allocation failed in add to cache\n");
            exit(1);
        }
        item->instruction = strdup(instruction); 
        item->result = result;  

        HASH_ADD_KEYPTR(hh, cache_table->instruction_table, instruction, strlen(item->instruction), item);
    }
}

process_control_block *search_cache_process(cache *cache_table, unsigned short int process_id) {
    hash_process *item = NULL;

    HASH_FIND(hh, cache_table->process_table, &process_id, sizeof(unsigned short int), item);

    if (item != NULL) {
        return item->process_pcb; 
    }
    return NULL; 
}

bool search_cache_instruction(cache *cache_table, char *instruction) {

    hash_instruction *item = NULL;

    HASH_FIND_STR(cache_table->instruction_table, instruction, item);
    
    return item != NULL;
}

void remove_cache_process(cache *cache_table, unsigned short int process_id) {
    hash_process *item = NULL;

    HASH_FIND(hh, cache_table->process_table, &process_id, sizeof(unsigned short int), item);

    if (item != NULL) {
        HASH_DEL(cache_table->process_table, item);  
        free(item);  
    } else {
        printf("Error: process_id not found in cache\n");
        exit(1);
    }
}

void remove_cache_instruction(cache *cache_table, char* instruction) {
    hash_instruction *item = NULL;

    HASH_FIND_STR(cache_table->instruction_table, instruction, item);

    if (item != NULL) {
        HASH_DEL(cache_table->instruction_table, item);  
        free(item->instruction); // Liberar memória alocada para a string
        free(item);  
    } else {
        printf("Error: instruction not found in cache\n");
        exit(1);
    }
}

void empty_cache(cache *cache_table) {
    hash_process *process_item, *tmp_process;
    hash_instruction *instruction_item, *tmp_instruction;

    HASH_ITER(hh, cache_table->process_table, process_item, tmp_process) {
        HASH_DEL(cache_table->process_table, process_item);
        free(process_item);
    }

    HASH_ITER(hh, cache_table->instruction_table, instruction_item, tmp_instruction) {
        HASH_DEL(cache_table->instruction_table, instruction_item);
        free(instruction_item->instruction); 
        free(instruction_item);
    }
}

unsigned short int get_result_cache_instruction(cache *cache_table, char* instruction) {
    hash_instruction *entry;

    HASH_FIND_STR(cache_table->instruction_table, instruction, entry);

    if (entry != NULL) {
        return entry->result; 
    } else {
        printf("Error: instruction not found in cache\n");
        exit(1); 
    }
}

void print_cache_instruction(cache *cache_table) {
    hash_instruction *item;
    printf("Printing instruction cache:\n");

    for (item = cache_table->instruction_table; item != NULL; item = item->hh.next) {
        printf("Instruction: %s, Result: %d\n", item->instruction, item->result);
    }
}

instruction_cache_item* init_instruction_cache_item() {
    instruction_cache_item* inst_cache_item = malloc(sizeof(instruction_cache_item));

    inst_cache_item->is_cached = false;
    inst_cache_item->reg_index = 0;
    inst_cache_item->result = -1;

    return inst_cache_item;
}