#include "cache.h"

void add_cache(cache **cache_table, unsigned short int address, unsigned short int data) {
    cache *item = NULL;

    HASH_FIND(hh, *cache_table, &address, sizeof(unsigned short int), item);

    if (item == NULL) {
        item = (cache *)malloc(sizeof(cache));
        if (item == NULL) {
            printf("memory allocation failed\n");
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
        printf("Item não encontrado\n");
    }
}

void print_cache(cache *cache_table) {
    cache *item;
    
    for (item = cache_table; item != NULL; item = (cache*)(item->hh.next)) {
        printf("address: %hu, data: %hu\n", item->address, item->data);
    }
}

void empty_cache(cache **cache_table) {
    cache *item, *tmp;
    
    HASH_ITER(hh, *cache_table, item, tmp) {
        HASH_DEL(*cache_table, item);
        free(item);
    }
}

// // Função principal para demonstrar as operações da cache
// int main() {
//     cache *cache_table = NULL;

//     // Adicionando itens na cache
//     adicionar_cache(&cache_table, 0x1A, 1234);
//     adicionar_cache(&cache_table, 0x2B, 5678);
//     adicionar_cache(&cache_table, 0x3C, 9012);

//     // Imprimindo a cache
//     printf("Itens na cache:\n");
//     imprimir_cache(cache_table);

//     // Buscando um item
//     cache *item = buscar_cache(cache_table, 0x2B);
//     if (item != NULL) {
//         printf("Item encontrado: Endereço = %hu, Dado = %hu\n", item->address, item->data);
//     } else {
//         printf("Item não encontrado\n");
//     }

//     // Removendo um item
//     remover_cache(&cache_table, 0x2B);
//     printf("Após remover o endereço 0x2B:\n");
//     imprimir_cache(cache_table);

//     // Limpando a cache
//     limpar_cache(&cache_table);

//     return 0;
// }