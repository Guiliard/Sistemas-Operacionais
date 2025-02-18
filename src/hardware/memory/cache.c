#include "cache.h"

void init_cache(cache *cache_table) {
    cache_table->process_table = NULL;
    cache_table->instruction_table = NULL;
}

void policy(cache* cache_table, type_policy policy_type) {
    switch (policy_type) {
        case FIFO_POLICY:
            fifo_policy(cache_table);
            break;
        case LEAST_RECENTLY_USED:
            lru_policy(cache_table);
            break;
        case RANDOM_REPLACEMENT:
            random_policy(cache_table);
            break;
    }
}

void fifo_policy(cache *cache_table) {

    hash_instruction *oldest;
    
    for (unsigned short int i =0; i < REMOVE_CACHE; i++) {

        oldest = cache_table->instruction_table;

        HASH_DEL(cache_table->instruction_table, oldest);

        free(oldest->instruction);
        free(oldest);
    }
}

void lru_policy(cache *cache_table) {
    
    hash_instruction *item, *lru_item = NULL;

    for (unsigned short int i = 0; i < REMOVE_CACHE; i++) {
        lru_item = NULL;

        for (item = cache_table->instruction_table; item != NULL; item = item->hh.next) {
            if (lru_item == NULL || item->frequency < lru_item->frequency) {
                lru_item = item;
            }
        }

        if (lru_item != NULL) {

            HASH_DEL(cache_table->instruction_table, lru_item);

            free(lru_item->instruction);
            free(lru_item);
        }
    }
}

void random_policy(cache *cache_table) {
    hash_instruction *item;

    for (unsigned short int i = 0; i < REMOVE_CACHE; i++) {
        unsigned short int random_index = rand() % (NUM_CACHE - i);  

        item = cache_table->instruction_table;
        
        for (unsigned short int j = 0; j < random_index; j++) {
            item = item->hh.next;
        }

        HASH_DEL(cache_table->instruction_table, item);

        free(item->instruction);
        free(item);
    }
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

void add_cache_instruction(cache *cache_table, const char *instruction, unsigned short int result, type_policy policy_type) {
    hash_instruction *item = NULL;

    HASH_FIND_STR(cache_table->instruction_table, instruction, item);

    if (item == NULL) {

        if (HASH_COUNT(cache_table->instruction_table) >= NUM_CACHE) {
            policy(cache_table, policy_type);
        }

        item = (hash_instruction *)malloc(sizeof(hash_instruction));
        if (item == NULL) {
            printf("Error: memory allocation failed in add to cache\n");
            exit(1);
        }
        item->instruction = strdup(instruction); 
        item->result = result;
        item->frequency = 0;  

        HASH_ADD_KEYPTR(hh, cache_table->instruction_table, item->instruction, strlen(item->instruction), item);
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

bool search_cache_instruction(cache *cache_table, const char *instruction) {

    hash_instruction *item = NULL;

    HASH_FIND_STR(cache_table->instruction_table, instruction, item);
    
    return item != NULL;
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
        entry->frequency++;
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
        printf("Instruction: %s, Result: %d, Frequency: %d\n", item->instruction, item->result, item->frequency);
    }
}

instruction_cache_item* init_instruction_cache_item() {
    instruction_cache_item* inst_cache_item = malloc(sizeof(instruction_cache_item));

    inst_cache_item->is_cached = false;
    inst_cache_item->reg_index = 0;
    inst_cache_item->result = -1;

    return inst_cache_item;
}