#include "cache.h"

void add_cache(cache **cache_table, unsigned short int address, unsigned short int data) {
    cache *item = NULL;

    HASH_FIND(hh, *cache_table, &address, sizeof(unsigned short int), item);

    if (item == NULL) {
        item = (cache *)malloc(sizeof(cache));
        if (item == NULL) {
            printf("Error: memory allocation failed in add to cache\n");
            exit(1);
        }
        item->address = address; 
        HASH_ADD(hh, *cache_table, address, sizeof(unsigned short int), item);
    }

    item->data = data;
}

cache *search_cache(cache *cache_table, unsigned short int address) {
    cache *item = NULL;

    HASH_FIND(hh, cache_table, &address, sizeof(unsigned short int), item);

    return item;
}

void remove_cache(cache **cache_table, unsigned short int address) {
    cache *item = search_cache(*cache_table, address);

    if (item != NULL) {
        HASH_DEL(*cache_table, item);
        free(item);
    } else {
        printf("Error: address not found in cache\n");
    }
}

void print_cache(cache *cache_table) {
    cache *item;
    
    for (item = cache_table; item != NULL; item = (cache*)(item->hh.next)) {
        printf("Address: %hu, Data: %hu\n", item->address, item->data);
    }
}

void empty_cache(cache **cache_table) {
    cache *item, *tmp;
    
    HASH_ITER(hh, *cache_table, item, tmp) {
        HASH_DEL(*cache_table, item);
        free(item);
    }
}