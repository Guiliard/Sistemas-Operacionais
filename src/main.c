#include "architecture.h"
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>

typedef struct {
    int id;                      // Número do programa
    cpu* cpu;                    // Referência para a CPU
    ram* memory_ram;             // Referência para a RAM
} thread_data;

void* thread_function(void* arg) {
    thread_data* data = (thread_data*)arg;

    printf("Thread %d iniciando execução do pipeline.\n", data->id);

    // Chamar a função init_pipeline para o programa associado
    init_pipeline(data->cpu, data->memory_ram, data->id);

    printf("Thread %d finalizou execução do pipeline.\n", data->id);
    return NULL;
}

int main() {
    //const int NUM_PROGRAMS = 4;
    cpu* cpu = malloc(sizeof(cpu));
    ram* memory_ram = malloc(sizeof(ram));
    disc* memory_disc = malloc(sizeof(disc));
    //pthread_t threads[NUM_PROGRAMS]; 
    //thread_data data[NUM_PROGRAMS]; 
    peripherals* peripherals = malloc(sizeof(peripherals));
    char filename[50];
    int pos;

    init_architecture(cpu, memory_ram, memory_disc, peripherals);

    for (unsigned short int i = 1; i <= 4; i++) {  
        snprintf(filename, sizeof(filename), "dataset/program%d.txt", i);
        char *program = read_program(filename);
        pos = load_program_on_ram(memory_ram, program);
        printf ("%d", pos);
        check_instructions_on_ram(cpu, memory_ram, pos, i);
        //data[i - 1].id = i;
        //data[i - 1].cpu = cpu;
        //data[i - 1].memory_ram = memory_ram;
        //print_ram(memory_ram);
        free(program); 
    }

    /*for (int i = 0; i < NUM_PROGRAMS; i++) {
        if (pthread_create(&threads[i], NULL, thread_function, &data[i]) != 0) {
            fprintf(stderr, "Erro ao criar thread %d\n", data[i].id);
            exit(EXIT_FAILURE);
        }
    }

    // Esperar todas as threads terminarem
    for (int i = 0; i < NUM_PROGRAMS; i++) {
        pthread_join(threads[i], NULL);
    }*/

    /*for (int i=1; i<5; i++) {
        init_pipeline(cpu, memory_ram, i);
    }*/
    init_pipeline(cpu, memory_ram, 1);
    init_pipeline(cpu, memory_ram, 2);
    //print_ram(memory_ram);
    
    return 0;
}