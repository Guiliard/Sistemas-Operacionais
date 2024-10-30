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

void control_unit(cpu* cpu, ram* memory_ram, type_of_instruction type, char* instruction) {
    if (type == LOAD) {
        load(cpu, instruction);
    } else if (type == STORE) {
        store(cpu, memory_ram, instruction);
    } else if (type == ADD) {
        add(cpu, instruction);
    } else if (type == SUB) {
        sub(cpu, instruction);
    } else if (type == MUL) {
        mul(cpu, instruction);
    } else if (type == DIV) {
        div_c(cpu, instruction);
    } else if (type == IF) {
        // if
    } else if (type == ELSE) {
        // else
    } else if (type == LOOP) {
        // loop
    } else {
        printf("Error: Unrecognized instruction\n");
    }
    
}

unsigned short int ula(unsigned short int operating_a, unsigned short int operating_b, ula_operation operation) {
    switch(operation) {
        case ADD_ULA:
            return operating_a + operating_b;

        case SUB_ULA:
            return operating_a - operating_b;

        case MUL_ULA:
            return operating_a * operating_b; 

        case DIV_ULA:
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

unsigned short int get_register_index(char* reg_name) {
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
    
    printf("Error: Invalid register name.\n");
    return 0;
}

unsigned short int verify_address(ram* memory_ram, char* address, unsigned short int num_positions) {
    unsigned short int address_without_a;
    
    address_without_a = atoi(address + 1);  

    if (address_without_a + num_positions > NUM_MEMORY) {
        printf("Error: Invalid memory address - out of bounds.\n");
        exit(1);
    }

    for (unsigned short int i = 0; i < num_positions; i++) {
        if (memory_ram->vector[address_without_a + i] != '\0') {
            printf("Error: Invalid memory address - position %d is already occupied.\n", address_without_a + i);
            exit(1);
        }
    }

    return address_without_a; 
}

void load (cpu* cpu, char* instruction) {

    char *token;
    char *register_name;
    unsigned short int value;

    token = strtok(instruction, " "); 

    if (strcmp(token, "LOAD") != 0) {
        printf("Error: Invalid instruction\n");
        exit(1);
    }

    token = strtok(NULL, " ");

    register_name = token;

    token = strtok(NULL, " ");
    value = atoi(token);

    cpu->core[0].registers[get_register_index(register_name)] = value;
}

void store (cpu* cpu, ram* memory_ram, char* instruction) {
    char *token;
    char *register_name1, *memory_address;
    char buffer[10]; 
    unsigned short int address, num_positions;

    token = strtok(instruction, " "); 

    if (strcmp(token, "STORE") != 0) {
        printf("Error: Invalid instruction\n");
        exit(1);
    }
    token = strtok(NULL, " ");

    register_name1 = token;

    token = strtok(NULL, " ");
   
    memory_address = token;

    unsigned short int register_value = cpu->core[0].registers[get_register_index(register_name1)];

    sprintf(buffer, "%d", register_value);  

    num_positions = strlen(buffer); 

    address = verify_address(memory_ram, memory_address, num_positions);

    strcpy(&memory_ram->vector[address], buffer);
}

void add(cpu* cpu, char* instruction) {
    char *token;
    char *register_name1, *register_name2;
    unsigned short int value;

    token = strtok(instruction, " "); 

    if (strcmp(token, "ADD") != 0) {
        printf("Error: Invalid instruction\n");
        exit(1);
    }

    token = strtok(NULL, " ");

    register_name1 = token;

    token = strtok(NULL, " ");
    
    unsigned short int result;

    if (isdigit(token[0])) {
        value = atoi(token);
        result = ula(cpu->core[0].registers[get_register_index(register_name1)], value, ADD_ULA);
        cpu->core[0].registers[get_register_index(register_name1)] = result;
    } else {
        register_name2 = token;
        result = ula(cpu->core[0].registers[get_register_index(register_name1)], 
                     cpu->core[0].registers[get_register_index(register_name2)], 
                     ADD_ULA);
        cpu->core[0].registers[get_register_index(register_name1)] = result;
    }
}


void sub(cpu* cpu, char* instruction) {
    char *token;
    char *register_name1, *register_name2;
    unsigned short int value;

    token = strtok(instruction, " "); 

    if (strcmp(token, "SUB") != 0) {
        printf("Error: Invalid instruction\n");
        exit(1);
    }

    token = strtok(NULL, " ");

    register_name1 = token;

    token = strtok(NULL, " ");
    
    unsigned short int result;

    if (isdigit(token[0])) {
        value = atoi(token);
        result = ula(cpu->core[0].registers[get_register_index(register_name1)], value, SUB_ULA);
        cpu->core[0].registers[get_register_index(register_name1)] = result;
    } else {
        register_name2 = token;
        result = ula(cpu->core[0].registers[get_register_index(register_name1)], 
                     cpu->core[0].registers[get_register_index(register_name2)], 
                     SUB_ULA);
        cpu->core[0].registers[get_register_index(register_name1)] = result;
    }
}

void mul(cpu* cpu, char* instruction) {
    char *token;
    char *register_name1, *register_name2;
    unsigned short int value;

    token = strtok(instruction, " "); 

    if (strcmp(token, "MUL") != 0) {
        printf("Error: Invalid instruction\n");
        exit(1);
    }

    token = strtok(NULL, " ");

    register_name1 = token;

    token = strtok(NULL, " ");
    
    unsigned short int result;

    if (isdigit(token[0])) {
        value = atoi(token);
        result = ula(cpu->core[0].registers[get_register_index(register_name1)], value, MUL_ULA);
        cpu->core[0].registers[get_register_index(register_name1)] = result;
    } else {
        register_name2 = token;
        result = ula(cpu->core[0].registers[get_register_index(register_name1)], 
                     cpu->core[0].registers[get_register_index(register_name2)], 
                     MUL_ULA);
        cpu->core[0].registers[get_register_index(register_name1)] = result;
    }
}

void div_c(cpu* cpu, char* instruction) {
    char *token;
    char *register_name1, *register_name2;
    unsigned short int value;

    token = strtok(instruction, " "); 

    if (strcmp(token, "DIV") != 0) {
        printf("Error: Invalid instruction\n");
        exit(1);
    }

    token = strtok(NULL, " ");

    register_name1 = token;

    token = strtok(NULL, " ");
    
    unsigned short int result;

    if (isdigit(token[0])) {
        value = atoi(token);
        result = ula(cpu->core[0].registers[get_register_index(register_name1)], value, DIV_ULA);
        cpu->core[0].registers[get_register_index(register_name1)] = result;
    } else {
        register_name2 = token;
        result = ula(cpu->core[0].registers[get_register_index(register_name1)], 
                     cpu->core[0].registers[get_register_index(register_name2)], 
                     DIV_ULA);
        cpu->core[0].registers[get_register_index(register_name1)] = result;
    }
}

