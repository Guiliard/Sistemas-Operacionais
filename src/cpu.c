#include "cpu.h"

#include "pipeline.h"

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


void control_unit(cpu* cpu, pipe* p) {
    p->result = 0;

    if (p->type == ADD) {
        p->result = add(cpu, p->instruction);
        p->num_instruction++;
    } else if (p->type == SUB) {
        p->result = sub(cpu, p->instruction);
        p->num_instruction++;
    } else if (p->type == MUL) {
        p->result = mul(cpu, p->instruction);
        p->num_instruction++;
    } else if (p->type == DIV) {
        p->result = div_c(cpu, p->instruction);
        p->num_instruction++;
    } else if (p->type == LOOP) {
        loop(cpu, p);
        p->num_instruction++;
    } else if (p->type == L_END) {
        loop_end(cpu, p);
    } else if(p->type == IF) {
        if_i(cpu,p);
        p->num_instruction++;
    } else if(p->type == I_END) {
        if_end(p);
        p->num_instruction++;
    } else if(p->type == ELSE) {
        else_i(cpu,p);
        p->num_instruction++;
    } else if(p->type == ELS_END) {
        else_end(p);
        p->num_instruction++;
    }
    else {
        p->num_instruction++;
    }
}


unsigned short int ula(unsigned short int operating_a, unsigned short int operating_b, type_of_instruction operation) {
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

    char *instruction_copy, *token, *register_name;
    unsigned short int value;

    instruction_copy = strdup(instruction);

    token = strtok(instruction_copy, " "); 

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
    char *instruction_copy, *token, *register_name1, *memory_address;
    char buffer[10]; 
    unsigned short int address, num_positions;

    instruction_copy = strdup(instruction);

    token = strtok(instruction_copy, " "); 

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

unsigned short int add(cpu* cpu, char* instruction) {
    char *instruction_copy, *token,*register_name1, *register_name2;
    unsigned short int value;

    instruction_copy = strdup(instruction);

    token = strtok(instruction_copy, " "); 

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
        result = ula(cpu->core[0].registers[get_register_index(register_name1)], value, ADD);
    } else {
        register_name2 = token;
        result = ula(cpu->core[0].registers[get_register_index(register_name1)], 
                     cpu->core[0].registers[get_register_index(register_name2)], 
                     ADD);
    }

    return result; 
}

unsigned short int sub(cpu* cpu, char* instruction) {
    char *instruction_copy, *token, *register_name1, *register_name2;
    unsigned short int value;

    instruction_copy = strdup(instruction);

    token = strtok(instruction_copy, " "); 

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
        result = ula(cpu->core[0].registers[get_register_index(register_name1)], value, SUB);
    } else {
        register_name2 = token;
        result = ula(cpu->core[0].registers[get_register_index(register_name1)], 
                     cpu->core[0].registers[get_register_index(register_name2)], 
                     SUB);
    }

    return result; 
}

unsigned short int mul(cpu* cpu, char* instruction) {
    char *instruction_copy, *token, *register_name1, *register_name2;
    unsigned short int value;

    instruction_copy = strdup(instruction);

    token = strtok(instruction_copy, " "); 

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
        result = ula(cpu->core[0].registers[get_register_index(register_name1)], value, MUL);
    } else {
        register_name2 = token;
        result = ula(cpu->core[0].registers[get_register_index(register_name1)], 
                     cpu->core[0].registers[get_register_index(register_name2)], 
                     MUL);
    }

    return result; 
}

unsigned short int div_c(cpu* cpu, char* instruction) {
    char *instruction_copy, *token, *register_name1, *register_name2;
    unsigned short int value;

    instruction_copy = strdup(instruction);

    token = strtok(instruction_copy, " "); 

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
        result = ula(cpu->core[0].registers[get_register_index(register_name1)], value, DIV);
    } else {
        register_name2 = token;
        result = ula(cpu->core[0].registers[get_register_index(register_name1)], 
                     cpu->core[0].registers[get_register_index(register_name2)], 
                     DIV);
    }

    return result;  
}

void if_i(cpu* cpu, pipe* p) {
    char *instruction_copy = strdup(p->instruction);
    char *token = strtok(instruction_copy, " ");
    p->has_if = true;

    if (strcmp(token, "IF") != 0) {
        printf("Error: Invalid instruction\n");
        exit(1);
    }

    token = strtok(NULL, " ");
    unsigned short int register1_value = get_register_index(token);
    register1_value = cpu->core[0].registers[register1_value];

    token = strtok(NULL, " ");
    const char *operator = token;

    token = strtok(NULL, " ");
    unsigned short int operand_value;
    if (isdigit(token[0])) {
        operand_value = atoi(token);
    } else {
        operand_value = get_register_index(token);
    }

    int result = 0;
    if (strcmp(operator, "==") == 0) {
        result = register1_value == operand_value;
    } else if (strcmp(operator, "!=") == 0) {
        result = register1_value != operand_value;
    } else if (strcmp(operator, "<=") == 0) {
        result = register1_value <= operand_value;
    } else if (strcmp(operator, ">=") == 0) {
        result = register1_value >= operand_value;
    } else if (strcmp(operator, ">") == 0) {
        result = register1_value > operand_value;
    } else if (strcmp(operator, "<") == 0) {
        result = register1_value < operand_value;
    } else {
        printf("Error: Invalid operator. Line %hd.\n",p->num_instruction + 1);
    }

    if (result == 0) {
        p->valid_if = false;
        while (1) {
            p->num_instruction++;
            p->instruction = instruc_fetch(cpu, p->mem_ram);

            p->type = instruc_decode(p->instruction, p->num_instruction);

            instruction_copy = strdup(p->instruction);
            token = strtok(instruction_copy, " "); 

            if (strcmp(token, "I_END") == 0)
                break;
        }
    } else {
        p->valid_if = true;
        p->running_if = true;
    }

    free(instruction_copy);
}

void if_end(pipe* p) {
    char *instruction_copy, *token;

    instruction_copy = strdup(p->instruction);

    token = strtok(instruction_copy, " "); 

    if (strcmp(token, "I_END") != 0) {
        printf("Error: Invalid instruction\n");
        exit(1);
    }

    p->running_if = false;
}

void else_i(cpu* cpu, pipe* p) {
    char *instruction_copy = strdup(p->instruction);
    char *token = strtok(instruction_copy, " ");

    if (strcmp(token, "ELSE") != 0) {
        printf("Error: Invalid instruction\n");
        exit(1);
    }

    if (p->has_if && !p->valid_if) {
        p->has_if = false;
    }
    else if (p->running_if) {
        printf("Error: Invalid instruction\n");
        exit(1);
    }
    else if (!p->has_if) {
        printf("Error: Invalid instruction. No IF before ELSE. Line %hd.\n",p->num_instruction + 1);
        exit(1);
    }
    else if (p->has_if && p->valid_if) {
        while (1) {
            p->num_instruction++;
            p->instruction = instruc_fetch(cpu, p->mem_ram);

            p->type = instruc_decode(p->instruction, p->num_instruction);
            instruction_copy = strdup(p->instruction);
            token = strtok(instruction_copy, " "); 

            if (strcmp(token, "ELS_END") == 0)
                break;
        }
    }
    free(instruction_copy);
}

void else_end(pipe* p) {
    char *instruction_copy = strdup(p->instruction);
    char *token = strtok(instruction_copy, " ");

    if (strcmp(token, "ELS_END") != 0) {
        printf("Error: Invalid instruction\n");
        exit(1);
    }

    p->has_if = false;
}

void loop(cpu* cpu, pipe* p) {
    char *instruction_copy, *token, *register_name;
    unsigned short int value;

    instruction_copy = strdup(p->instruction);

    token = strtok(instruction_copy, " "); 

    if (strcmp(token, "LOOP") != 0) {
        printf("Error: Invalid instruction\n");
        exit(1);
    }

    if (!p->loop) {
        token = strtok(NULL, " ");
        if (isdigit(token[0])) {
            value = atoi(token);
            if (value == 0) {
                printf("Error: Loop value can't be 0. Line %hd.\n",p->num_instruction + 1);
                exit(1);
            }
        } else {
            register_name = token;
            value = cpu->core[0].registers[get_register_index(register_name)];
            if (value == 0) {
                printf("Error: Loop value can't be 0. Line %hd.\n",p->num_instruction + 1);
                exit(1);
            }

        }
        p->loop_value = value;
        p->loop_start = p->num_instruction;
        p->loop = true;
    }
}

void loop_end(cpu* cpu, pipe* p) {
    char *instruction_copy, *token;

    instruction_copy = strdup(p->instruction);

    token = strtok(instruction_copy, " "); 

    if (strcmp(token, "L_END") != 0) {
        printf("Error: Invalid instruction.\n");
        exit(1);
    }

    int decrease = p->num_instruction - p->loop_start + 1;
    p->loop_value--;
    if (p->loop_value == 0) {
        p->loop = false;
        p->loop_start = 0;
        p->num_instruction++;
    }
    else {
        for (int i=0; i<decrease; i++) {
            decrease_pc(cpu);
        }
        p->num_instruction = p->loop_start;
    }
}

void decrease_pc(cpu* cpu) {
    cpu->core[0].PC--;
}

char* instruc_fetch(cpu* cpu, ram* memory) {
    char* instruction = get_line_of_program(memory->vector, cpu->core[0].PC);
    cpu->core[0].PC++;

    return instruction;
}

type_of_instruction instruc_decode(char* instruction, unsigned short int num_instruction) {
    type_of_instruction type = verify_instruction(instruction, num_instruction);

    if (type == INVALID) {
        exit(1);    
    } else {
        return type;
    }
}