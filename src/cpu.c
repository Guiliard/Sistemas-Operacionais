#include "cpu.h"

void init_cpu(cpu* cpu) {
    cpu->core = malloc(NUM_CORES * sizeof(core));
    if (cpu->core == NULL) {
        printf("memory allocation failed in cpu\n");
        exit(1);
    }

    for (unsigned short int i = 0; i < NUM_CORES; i++) {
        cpu->core[i].registers = malloc(NUM_REGISTERS * sizeof(unsigned short int));
        if (cpu->core[i].registers == NULL) {
            printf("memory allocation failed in cpu->core[%d].registers\n", i);
            exit(1);
        }
        cpu->core[i].PC = 0;  
        for (unsigned short int j = 0; j < NUM_REGISTERS; j++) {
            cpu->core[i].registers[j] = 0;
        }
    }
}

void control_unit(cpu* cpu, char** instruction) {
    if (strcmp(instruction[0], "LOAD") == 0) {
        load(cpu, instruction);
    } else if (strcmp(instruction[0], "STORE") == 0) {
        // store
    } else if (strcmp(instruction[0], "ADD") == 0) {
        // add
    } else if (strcmp(instruction[0], "SUB") == 0) {
        // sub
    } else if (strcmp(instruction[0], "MUL") == 0) {
        // mul
    } else if (strcmp(instruction[0], "DIV") == 0) {
        // div
    } else if (strcmp(instruction[0], "IF") == 0) {
        // if
    } else if (strcmp(instruction[0], "ELSE") == 0) {
        // else
    } else if (strcmp(instruction[0], "LOOP") == 0) {
        // loop
    } else {
        printf("Error: Unrecognized instruction\n");
    }
    
}

unsigned short int ula(unsigned short int operating_a, unsigned short int operating_b, ula_operation operation) {
    switch(operation) {
        case ADD:
            return operating_a + operating_b;

        case SUB:
            return operating_a - operating_b;

        case MUL:
            return operating_a * operating_b; 

        case DIV:
            if (operating_b == 0) {
                printf("Error: Division by zero.\n");
                return 0; 
            }
            return operating_a / operating_b;

        default:
            printf("Error: Invalid operation.\n");
            return 0;
    }
}

int get_register_index(const char* reg_name) {
    char* register_names[] = {
        "A0", "B0", "C0", "D0", "E0", "F0", "G0", "H0",
        "I0", "J0", "K0", "L0", "M0", "N0", "O0", "P0",
        "A1", "B1", "C1", "D1", "E1", "F1", "G1", "H1",
        "I1", "J1", "K1", "L1", "M1", "N1", "O1", "P1"
    };

    for (int i = 0; i < NUM_REGISTERS; i++) {
        if (strcmp(reg_name, register_names[i]) == 0) {
            return i;
        }
    }
    return -1;
}

void load (cpu* cpu, char** instruction) {
    int register_index = get_register_index(instruction[1]);
    if (register_index == -1) {
        printf("Error: Invalid register name %s\n", instruction[1]);
        return;
    }

    unsigned short int value = (unsigned short int) atoi(instruction[2]);
    cpu->core[0].registers[register_index] = value;
}

// void store(CPU* cpu, si source_register, si offset, si base_register, RAM* memory) {
//     si address = cpu->registers[base_register] + offset;

//     memory->vector[address] = cpu->registers[source_register]; // memory
// }

// void writeondisk(disc* disco, si end1, si end2, si valor) {
//     if (end1>=0 && end1 < NUM_MEMORY && end2>=0 && end2 < NUM_MEMORY)
//         disco->matriz[end1][end2] = valor;
//     else
//         printf("Memoria de disco indisponivel");
// }

// si readfromdisk(disc* disco, si end1, si end2) {
//     if (end1>=0 && end1 < NUM_MEMORY && end2>=0 && end2 < NUM_MEMORY)
//         return disco->matriz[end1][end2];
//     else {
//         printf("Memoria de disco indisponivel");
//         return -1;
//     }
// }

// /*char* initdisk() {
//     char *disco = (char*) malloc(DISCO_SIZE);
    
//     if (disco == NULL) {
//         printf("Erro ao alocar memoria para o disco.\n");
//         exit(1);
//     }
//     return disco;
// }

// void writeondisk(char* disco, int endereco, int valor) {
//     if (endereco>=0 && endereco < DISCO_SIZE)
//         disco[endereco] = valor;
//     else
//         printf("Memoria de disco indisponivel");
// }

// char readfromdisk(char* disco, int endereco) {
//     if (endereco>=0 && endereco < DISCO_SIZE)
//         return disco[endereco];
//     else {
//         printf("Memoria de disco indisponivel");
//         return -1;
//     }
// }*/




// // // pipeline
// // void instruction_fetch(CPU* cpu, RAM* memory){
//    // si instruction = cpu->current_instruction;

// // }

// // void instruction_decode(CPU* cpu) {

// // }

// // void execute(CPU* cpu){
//         // instruction_fetch(cpu, memory);
//         // instruction_decode(cpu);
//         // execute(cpu);
//         // memory_access(cpu, memory);
//         // write_back(cpu);
// // }

// // void memory_access(CPU* cpu, RAM* memory) {
// //     load
// //     store
// // }

// // void write_back(CPU* cpu){
//     // cpu->registers[cpu->rd] = cpu->result;
// // }

// // void pipeline_cycle(CPU* cpu, RAM* memory){

// // }