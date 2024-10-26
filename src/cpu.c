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
    }
}

void control_unit(opcode command) {
    switch(command) {
    case 0: // LOAD
    break;
    case 1: // STORE
    break;
    case 2: // ADD
    break;
    case 3: // SUB
    break;
    case 4: // MUL
    break;
    case 5: // DIV
    break;
    case 6: // IF
    break;
    case 7: // ELSE
    break;
    case 8: // LOOP
    break;
    default:
    break;
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
    }
}

// void jump (CPU* cpu, si address) {
//     cpu->PC = address;
// }

// void load(CPU* cpu, si destination_register, si offset, si base_register, RAM* memory) {
//     si address = cpu->registers[base_register] + offset;

//     cpu->registers[destination_register] = memory->vector[address];
// }

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