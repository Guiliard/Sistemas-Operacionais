#include "architecture.h"

int main() {
    cpu* cpu = malloc(sizeof(cpu));;
    ram* memory_ram = malloc(sizeof(ram));;
    disc* memory_disc = malloc(sizeof(disc));;
    peripherals* peripherals = malloc(sizeof(peripherals));

    init_architecture(cpu, memory_ram, memory_disc, peripherals);

    char *program = read_program("dataset/program.txt");

    load_program_on_ram(memory_ram, program);

    check_instructions_on_ram(memory_ram);

    init_pipeline(cpu, memory_ram);

    print_ram(memory_ram);
    
    free(program);  
    return 0;
}
