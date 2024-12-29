#include "architecture.h"

void init_architecture(cpu* cpu, ram* memory_ram, disc* memory_disc, peripherals* peripherals) {
    init_cpu(cpu);
    init_ram(memory_ram);
    init_disc(memory_disc);
    init_peripherals(peripherals);
}

int load_program_on_ram(ram* memory_ram, char* program) {
    unsigned short int num_caracters = strlen(program);
    int used_memory = 0;

    // Calcular quanto da memória RAM já está ocupada
    for (int i = 0; i < NUM_MEMORY; i++) {
        if (memory_ram->vector[i] != '\0') {
            used_memory++;
        } else {
            break; // Parar ao encontrar o final da memória usada
        }
    }

    // Verificar se há espaço suficiente para o novo programa mais o separador '&'
    if (used_memory + num_caracters + 3 > NUM_MEMORY) { // +1 para o caractere '&'
        printf("Error: Not enough space in RAM for the program.\n");
        exit(1);
    }

    // Encontrar a primeira posição livre na memória RAM
    int start_position = used_memory;

    // Carregar o programa na RAM
    for (unsigned short int i = 0; i < num_caracters; i++) {
        memory_ram->vector[start_position + i] = program[i];
    }

    // Adicionar o caractere '&' ao final do programa
    memory_ram->vector[start_position + num_caracters] = '\n';
    memory_ram->vector[start_position + num_caracters + 1] = '&';
    memory_ram->vector[start_position + num_caracters + 2] = '\n';
    memory_ram->vector[start_position + num_caracters + 3] = '\0';

    // Retornar a posição inicial onde o programa foi carregado
    return start_position;
}

void check_instructions_on_ram(cpu* cpu, ram* memory_ram, int pos, unsigned short int nump) {   
    char* line;
    unsigned short int num_line = first_line(memory_ram->vector,nump);
    unsigned short int num = count_lines(memory_ram->vector,nump);
    num += num_line;
    change_pc_core(cpu, nump, pos);

    printf("\n%hd ate %hd\n\n",num_line,num);

    while (num_line < num) {
        line = get_line_of_program(memory_ram->vector, num_line);
        verify_instruction(line, num_line);
        num_line++;
    }
}

void init_pipeline(cpu* cpu, ram* memory_ram, unsigned short int nump) {
    pipe p;
    unsigned short int num_lines = 0;
    p.num_instruction = 0;
    p.mem_ram = memory_ram;

    num_lines = count_lines(memory_ram->vector,nump);
    printf ("%hd",num_lines);
    printf("Number of instructions: %hd\n", num_lines);

    while (p.num_instruction < num_lines) {

        p.instruction = instruction_fetch(cpu, memory_ram, nump);

        p.type = instruction_decode(p.instruction, p.num_instruction);

        execute(cpu, &p);

        memory_access(cpu, memory_ram, p.type, p.instruction);

        write_back(cpu, p.type, p.instruction, p.result, nump);

    }
}