#ifndef CACHE_H
#define CACHE_H

#include "uthash.h"
#include "libs.h"

typedef struct cache {
    unsigned short int address;//chave
    unsigned short int data;//valor
    UT_hash_handle hh;
} cache;

#endif