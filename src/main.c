#include "architecture.h"

int main() {
    cpu* cpu = malloc(sizeof(cpu));;
    ram* memory_ram = malloc(sizeof(ram));;
    disc* memory_disc = malloc(sizeof(disc));;
    peripherals* peripherals = malloc(sizeof(peripherals));

    init_architecture(cpu, memory_ram, memory_disc, peripherals);

    char *program = read_program("dataset/program.txt");  
    printf("program:\n%s\n", program);

    unsigned short int num_lines = count_lines(program);

    printf("number of lines: %hu\n", num_lines);

    for (int i = 0; i < num_lines; i++) {
        char* line = get_line_of_program(program, i);
        printf("line %d: %s\n", i, line);
    }

    load_program_on_ram(memory_ram, program);

    printf("RAM:\n");

    print_ram(memory_ram);
    
    free(program);  
    return 0;
}
