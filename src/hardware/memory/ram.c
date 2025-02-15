#include "ram.h"

void init_ram(ram* memory_ram) {
    memory_ram->vector = malloc(NUM_MEMORY * sizeof(char));

    if (memory_ram->vector == NULL) {
            printf("Error: memory allocation failed in ram\n");
            exit(1);
        }
    
    for (unsigned short int i = 0; i < NUM_MEMORY; i++) {
        memory_ram->vector[i] = '\0'; 
    }
}

void print_ram(ram* memory_ram) {
    for (unsigned short int i = 0; i < NUM_MEMORY; i++) {
        if(memory_ram->vector[i] == '\0') {
            printf("_");
        } else {
            printf("%c", memory_ram->vector[i]);
        }
    }
}

unsigned short int verify_address(char* address, unsigned short int num_positions) {
    unsigned short int address_without_a;
    
    address_without_a = atoi(address + 1);  

    if (address_without_a + num_positions > NUM_MEMORY) {
        printf("Error: Invalid memory address - out of bounds.\n");
        exit(1);
    }

    return address_without_a; 
}

void write_ram(ram* memory_ram, unsigned short int address, char* buffer) {
    strcpy(&memory_ram->vector[address], buffer);
}

void reset_ram(ram* memory_ram) {
    for (unsigned short int i = 0; i < NUM_MEMORY; i++) {
        memory_ram->vector[i] = '\0'; 
    }
}