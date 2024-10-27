#include "architecture.h"

void init_architecture(cpu* cpu, ram* memory_ram, disc* memory_disc, peripherals* peripherals) {
    init_cpu(cpu);
    init_ram(memory_ram);
    init_disc(memory_disc);
    init_peripherals(peripherals);
}

void load_program_on_ram(ram* memory_ram, char* program) {
    unsigned short int num_caracters = strlen(program);

    for (unsigned short int i = 0; i < num_caracters; i++) {
        memory_ram->vector[i] = program[i];
    }
}