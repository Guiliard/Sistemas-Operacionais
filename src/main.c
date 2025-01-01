#include "architecture.h"
#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>

// Estrutura para passar argumentos para cada thread
typedef struct {
    cpu* cpu;
    ram* memory_ram;
    char* program;  // Programa a ser executado
    int core_id;    // ID do núcleo associado
} thread_args;

// Função executada por cada thread
void* thread_function(void* args) {
    thread_args* t_args = (thread_args*)args;

    printf("Core %d: Iniciando execução do programa:\n%s\n", t_args->core_id, t_args->program);

    // Executa o pipeline no núcleo associado
    init_pipeline(t_args->cpu, t_args->memory_ram, t_args->program, t_args->core_id);

    printf("Core %d: Execução finalizada.\n", t_args->core_id);

    pthread_exit(NULL);
}

int main() {
    // Inicialização das estruturas
    cpu* cpu = malloc(sizeof(cpu));
    ram* memory_ram = malloc(sizeof(ram));
    disc* memory_disc = malloc(sizeof(disc));
    peripherals* peripherals = malloc(sizeof(peripherals));
    queue_start* queue_start = malloc(sizeof(queue_start));
    queue_end* queue_end = malloc(sizeof(queue_end));
    queue_block* queue_block = malloc(sizeof(queue_block));

    char filename[25];

    init_architecture(cpu, memory_ram, memory_disc, peripherals, queue_start, queue_end, queue_block);

    // Carrega os programas na RAM
    for (unsigned short int index_program = 0; index_program < NUM_PROGRAMS; index_program++) {
        sprintf(filename, "dataset/program%d.txt", index_program);
        char* program = read_program(filename);
        load_program_on_ram(memory_ram, program);
        free(program);
    }

    check_instructions_on_ram(memory_ram);
    print_ram(memory_ram);
    printf("\n");

    // Popula a fila inicial com programas da RAM
    populate_queue_start(queue_start, memory_ram);

    // Criação das threads para executar os programas
    pthread_t threads[NUM_PROGRAMS];
    thread_args t_args[NUM_PROGRAMS];

    for (int i = 0; i < NUM_PROGRAMS; i++) {
        t_args[i].cpu = cpu;
        t_args[i].memory_ram = memory_ram;
        t_args[i].program = queue_start->initial_queue[i].program;  // Obtém o programa da fila inicial
        t_args[i].core_id = i % NUM_CORES;  // Associa cada programa a um núcleo disponível

        if (pthread_create(&threads[i], NULL, thread_function, &t_args[i]) != 0) {
            perror("Erro ao criar thread");
            exit(1);
        }
    }

    // Aguarda todas as threads finalizarem
    for (int i = 0; i < NUM_PROGRAMS; i++) {
        pthread_join(threads[i], NULL);
    }

    // Libera memória alocada
    free(cpu);
    free(memory_ram);
    free(memory_disc);
    free(peripherals);
    free(queue_start);
    free(queue_end);
    free(queue_block);

    return 0;
}
