#include "threads.h"
#include <time.h>

int main() {
    clock_t start_time, end_time;
    double time_taken;

    start_time = clock();

    cpu* cpu = malloc(sizeof(cpu));
    ram* memory_ram = malloc(sizeof(ram));
    disc* memory_disc = malloc(sizeof(disc));
    peripherals* peripherals = malloc(sizeof(peripherals));
    queue_start* queue_start = malloc(sizeof(queue_start));
    queue_end* queue_end = malloc(sizeof(queue_end));

    char filename[25];

    init_architecture(cpu, memory_ram, memory_disc, peripherals, queue_start, queue_end);

    for (unsigned short int index_program = 0; index_program < NUM_PROGRAMS; index_program++) {
        sprintf(filename, "dataset/program%d.txt", index_program);
        char* program = read_program(filename);
        load_program_on_ram(memory_ram, program);
        free(program);
    }

    check_instructions_on_ram(memory_ram);
    print_ram(memory_ram);
    printf("\n");

    populate_queue_start(queue_start, memory_ram);
    check_resources_on_queue_start(queue_start);

    reset_ram(memory_ram);

    initialize_log_s_file();
    initialize_log_e_file();
    initialize_log_b_file();

    init_threads(cpu, memory_ram, queue_start, queue_end);

    print_ram(memory_ram);

    free_architecture(cpu, memory_ram, memory_disc, peripherals, queue_start, queue_end);

    end_time = clock();
    time_taken = ((double) (end_time - start_time)) / CLOCKS_PER_SEC;
    printf("\nExecution time: %.4f seconds\n", time_taken);

    return 0;
}
