#include "CPU.h"

void init_core(Core *c) {
    for (int i = 0; i < 32; i++) {
        c->registers[i] = 0;
    }
    c->PC = 0;
    c->IR = 0;
}

void set_register(Core *c, int reg_num, int value) {
    if (reg_num >= 0 && reg_num < 32) {
        c->registers[reg_num] = value;
    }
}

int get_register(Core *c, int reg_num) {
    if (reg_num >= 0 && reg_num < 32) {
        return c->registers[reg_num];
    }
    return 0; 
}

void print_registers(Core *c) {
    for (int i = 0; i < 32; i++) {
        printf("Register %d: %d\n", i, c->registers[i]);
    }
    printf("PC: %d\n", c->PC);
    printf("IR: %d\n", c->IR);
}

// Implementação da Memória
void init_memory(Memory *mem) {
    for (int i = 0; i < MEMORY_SIZE; i++) {
        mem->data[i] = 0;
    }
}

void write_memory(Memory *mem, int address, int value) {
    if (address >= 0 && address < MEMORY_SIZE) {
        mem->data[address] = value;
    } else {
        printf("Erro: Endereço de memória inválido para escrita.\n");
    }
}

int read_memory(Memory *mem, int address) {
    if (address >= 0 && address < MEMORY_SIZE) {
        return mem->data[address];
    } else {
        printf("Erro: Endereço de memória inválido para leitura.\n");
        return 0;
    }
}