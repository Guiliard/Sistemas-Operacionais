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
    printf("\n   -------------------------------------------------------------------------------------------------\n");
    printf("   |\t\t\t\t\t\tREGISTRADORES DA CPU\t\t\t\t|\n");
    printf("   -------------------------------------------------------------------------------------------------\n");
    printf("   |\tRegistrador\t|\tValor\t|\n");
    printf("   -------------------------------------------------------------------------------------------------\n");
    
    for (int i = 0; i < 32; i++) {
        printf("   |\t   R%d\t\t|\t  %d\t|\n", i, c->registers[i]);
    }
    
    printf("   -------------------------------------------------------------------------------------------------\n");
    printf("   |\tPC\t\t|\t  %d\t|\n", c->PC);
    printf("   |\tIR\t\t|\t  %d\t|\n", c->IR);
    printf("   -------------------------------------------------------------------------------------------------\n");
}


void init_memory(Memory *mem) {
    for (int i = 0; i < MEMORY_SIZE; i++) {
        mem->data[i] = 0;
    }
}


void print_memory(Memory *mem) {
    printf("\n   -------------------------------------------------------------------------------------------------\n");
    printf("   |\t\t\t\t\t\tMEMÓRIA PRINCIPAL\t\t\t\t|\n");
    printf("   -------------------------------------------------------------------------------------------------\n");
    printf("   |\tEndereço\t|\tValor\t|\n");
    printf("   -------------------------------------------------------------------------------------------------\n");
    
    for (int i = 0; i < MEMORY_SIZE; i += 4) {
        printf("   |\t  %d-%d\t\t|\t  %d\t|\n", i, i+3, mem->data[i]);
    }
    
    printf("   -------------------------------------------------------------------------------------------------\n");
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
