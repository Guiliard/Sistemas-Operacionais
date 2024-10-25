#include "cpu.h"

void init_cpu(cpu* cpu) {
    cpu->core = malloc(NUM_CORES * sizeof(unsigned short int));
    if (cpu->core == NULL) {
        printf("memory allocation failed in cpu\n");
        exit(1);
    }
    cpu->registers = malloc(NUM_REGISTERS * sizeof(unsigned short int));
    if (cpu->registers == NULL) {
        printf("memory allocation failed in cpu\n");
        exit(1);
    }
    cpu->PC = 0;
}

// void control_unit(si opcode) {
//     switch(opcode) {
//     case 0: // ADD
//     // Instruções para soma
//     break;
//     case 1: // SUB
//     // Instruções para subtração
//     break;
//     case 2: // AND
//     // Instruções para AND
//     break;
//     case 3: // OR
//     // Instruções para OR
//     break;
//     case 4: // XOR
//     // Instruções para XOR
//     break;
//     case 5: // SLL
//     // Instruções para SLL
//     break;
//     case 6: // SRL
//     // Instruções para SRL
//     break;
//     default:
//     // Instruções para instruções não reconhecidas 
//     break;
//     }
// }

// si ula_operation(si operating_a, si operating_b, si operation) {
//     switch(operation) {
//         case 0: // ADD
//         return operating_a + operating_b;
//         break;
//         case 1: // SUB
//         return operating_a - operating_b;
//         break;
//         case 2: // AND
//         return operating_a & operating_b;
//         break;
//         case 3: // OR
//         return operating_a | operating_b;
//         break;
//         case 4: // XOR
//         return operating_a ^ operating_b;
//         break;
//         case 5: // SLL
//         return operating_a << operating_b;
//         break;
//         case 6: // SRL
//         return operating_a >> operating_b;
//         break;
//         default:
//         return 0; 
//         break;
//     }
// }

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