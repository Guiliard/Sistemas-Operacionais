#include "control_unit.h"
#include "pipeline.h"

void control_unit(cpu* cpu, char* program, instruction_processor* instr_processor, unsigned short int index_core) {
    instr_processor->result = 0;

    if (instr_processor->type == ADD) {
        instr_processor->result = add(cpu, instr_processor->instruction, index_core);
        instr_processor->num_instruction++;
    } else if (instr_processor->type == SUB) {
        instr_processor->result = sub(cpu, instr_processor->instruction, index_core);
        instr_processor->num_instruction++;
    } else if (instr_processor->type == MUL) {
        instr_processor->result = mul(cpu, instr_processor->instruction, index_core);
        instr_processor->num_instruction++;
    } else if (instr_processor->type == DIV) {
        instr_processor->result = div_c(cpu, instr_processor->instruction, index_core);
        instr_processor->num_instruction++;
    } else if (instr_processor->type == LOOP) {
        loop(cpu, instr_processor, index_core);
        instr_processor->num_instruction++;
    } else if (instr_processor->type == L_END) {
        loop_end(cpu, instr_processor, index_core);
    } else if(instr_processor->type == IF) {
        if_i(cpu, program, instr_processor, index_core);
        instr_processor->num_instruction++;
    } else if(instr_processor->type == I_END) {
        if_end(instr_processor);
        instr_processor->num_instruction++;
    } else if(instr_processor->type == ELSE) {
        else_i(cpu, program, instr_processor, index_core);
        instr_processor->num_instruction++;
    } else if(instr_processor->type == ELS_END) {
        else_end(instr_processor);
        instr_processor->num_instruction++;
    }
    else {
        instr_processor->num_instruction++;
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

void trim(char* str) {
    char* end;

    while (isspace((unsigned char)*str)) str++;

    if (*str == 0) return;

    end = str + strlen(str) - 1;
    while (end > str && isspace((unsigned char)*end)) end--;

    *(end + 1) = '\0';
}


unsigned short int get_register_index(char* reg_name) {
    
    char* register_names[] = {
        "A0", "B0", "C0", "D0", "E0", "F0", "G0", "H0",
        "I0", "J0", "K0", "L0", "M0", "N0", "O0", "P0",
        "A1", "B1", "C1", "D1", "E1", "F1", "G1", "H1",
        "I1", "J1", "K1", "L1", "M1", "N1", "O1", "P1"
    };

    for (unsigned short int i = 0; i < NUM_REGISTERS; i++) {
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

void load (cpu* cpu, char* instruction, process_control_block* pcb, unsigned short int index_core) {

    char *instruction_copy, *token, *register_name;
    unsigned short int value, register_index;

    instruction_copy = strdup(instruction);

    token = strtok(instruction_copy, " "); 
    trim(token);

    if (strcmp(token, "LOAD") != 0) {
        printf("Error: Invalid instruction - LOAD\n");
        exit(1);
    }

    token = strtok(NULL, " ");
    trim(token);

    register_name = token;

    token = strtok(NULL, " ");
    trim(token);

    value = atoi(token);

    trim(register_name);
    register_index = get_register_index(register_name);

    // AQUI PEGA OS REGISTRADORES USADOS

    cpu->core[index_core].registers[register_index] = value;
}

void store (cpu* cpu, ram* memory_ram, char* instruction, unsigned short int index_core) {
    char *instruction_copy, *token, *register_name, *memory_address;
    char buffer[10]; 
    unsigned short int address, num_positions, register_index, register_value;

    instruction_copy = strdup(instruction);

    token = strtok(instruction_copy, " "); 
    trim(token);

    if (strcmp(token, "STORE") != 0) {
        printf("Error: Invalid instruction - STORE\n");
        exit(1);
    }

    token = strtok(NULL, " ");
    trim(token);

    register_name = token;

    token = strtok(NULL, " ");
    trim(token);

    memory_address = token;

    trim(register_name);
    register_index = get_register_index(register_name);

    register_value = cpu->core[index_core].registers[register_index];

    sprintf(buffer, "%d", register_value);  

    num_positions = strlen(buffer); 

    address = verify_address(memory_ram, memory_address, num_positions);

    strcpy(&memory_ram->vector[address], buffer);
}

unsigned short int add(cpu* cpu, char* instruction, unsigned short int index_core) {
    char *instruction_copy, *token,*register_name1, *register_name2;
    unsigned short int value, register_index1, register_index2, result;

    instruction_copy = strdup(instruction);

    token = strtok(instruction_copy, " "); 
    trim(token);

    if (strcmp(token, "ADD") != 0) {
        printf("Error: Invalid instruction - ADD\n");
        exit(1);
    }

    token = strtok(NULL, " ");
    trim(token);

    register_name1 = token;

    token = strtok(NULL, " ");
    trim(token);

    if (isdigit(token[0])) {
        value = atoi(token);

        trim(register_name1);
        register_index1 = get_register_index(register_name1);

        result = ula(cpu->core[index_core].registers[register_index1], value, ADD);
    } else {
        register_name2 = token;

        trim(register_name1);
        trim(register_name2);
        register_index1 = get_register_index(register_name1);
        register_index2 = get_register_index(register_name2);

        result = ula(cpu->core[index_core].registers[register_index1], 
                     cpu->core[index_core].registers[register_index2], 
                     ADD);
    }

    return result; 
}

unsigned short int sub(cpu* cpu, char* instruction, unsigned short int index_core) {
    char *instruction_copy, *token, *register_name1, *register_name2;
    unsigned short int value, register_index1, register_index2, result;

    instruction_copy = strdup(instruction);

    token = strtok(instruction_copy, " "); 
    trim(token);

    if (strcmp(token, "SUB") != 0) {
        printf("Error: Invalid instruction - SUB\n");
        exit(1);
    }

    token = strtok(NULL, " ");
    trim(token);

    register_name1 = token;

    token = strtok(NULL, " ");
    trim(token);

    if (isdigit(token[0])) {
        value = atoi(token);

        trim(register_name1);
        register_index1 = get_register_index(register_name1);

        result = ula(cpu->core[index_core].registers[register_index1], value, SUB);
    } else {
        register_name2 = token;

        trim(register_name1);
        trim(register_name2);
        register_index1 = get_register_index(register_name1);
        register_index2 = get_register_index(register_name2);

        result = ula(cpu->core[index_core].registers[register_index1], 
                     cpu->core[index_core].registers[register_index2], 
                     SUB);
    }

    return result; 
}

unsigned short int mul(cpu* cpu, char* instruction, unsigned short int index_core) {
    char *instruction_copy, *token, *register_name1, *register_name2;
    unsigned short int value, register_index1, register_index2, result;

    instruction_copy = strdup(instruction);

    token = strtok(instruction_copy, " ");
    trim(token); 

    if (strcmp(token, "MUL") != 0) {
        printf("Error: Invalid instruction - MUL\n");
        exit(1);
    }

    token = strtok(NULL, " ");
    trim(token);

    register_name1 = token;

    token = strtok(NULL, " ");
    trim(token);

    if (isdigit(token[0])) {
        value = atoi(token);

        trim(register_name1);
        register_index1 = get_register_index(register_name1);

        result = ula(cpu->core[index_core].registers[register_index1], value, MUL);
    } else {
        register_name2 = token;

        trim(register_name1);
        trim(register_name2);
        register_index1 = get_register_index(register_name1);
        register_index2 = get_register_index(register_name2);

        result = ula(cpu->core[index_core].registers[register_index1], 
                     cpu->core[index_core].registers[register_index2], 
                     MUL);
    }

    return result; 
}

unsigned short int div_c(cpu* cpu, char* instruction, unsigned short int index_core) {
    char *instruction_copy, *token, *register_name1, *register_name2;
    unsigned short int value, register_index1, register_index2, result;

    instruction_copy = strdup(instruction);

    token = strtok(instruction_copy, " "); 
    trim(token);

    if (strcmp(token, "DIV") != 0) {
        printf("Error: Invalid instruction - DIV\n");
        exit(1);
    }

    token = strtok(NULL, " ");
    trim(token);

    register_name1 = token;

    token = strtok(NULL, " ");
    trim(token);

    if (isdigit(token[0])) {
        value = atoi(token);

        trim(register_name1);
        register_index1 = get_register_index(register_name1);

        result = ula(cpu->core[index_core].registers[register_index1], value, DIV);
    } else {
        register_name2 = token;

        trim(register_name1);
        trim(register_name2);
        register_index1 = get_register_index(register_name1);
        register_index2 = get_register_index(register_name2);

        result = ula(cpu->core[index_core].registers[register_index1], 
                     cpu->core[index_core].registers[register_index2], 
                     DIV);
    }

    return result;  
}

void if_i(cpu* cpu, char* program, instruction_processor* instr_processor, unsigned short int index_core) {
    char* instruction_copy, *token, *operator;
    unsigned short int register_value, operand_value;
    
    instruction_copy = strdup(instr_processor->instruction);

    token = strtok(instruction_copy, " ");
    trim(token);

    instr_processor->has_if = true;

    if (strcmp(token, "IF") != 0) {
        printf("Error: Invalid instruction - IF\n");
        exit(1);
    }

    token = strtok(NULL, " ");
    trim(token);

    register_value = get_register_index(token);

    register_value = cpu->core[index_core].registers[register_value];

    token = strtok(NULL, " ");
    trim(token);

    operator = token;

    token = strtok(NULL, " ");
    trim(token);

    if (isdigit(token[0])) {
        operand_value = atoi(token);
    } else {
        operand_value = get_register_index(token);
    }

    int result = 0;
    if (strcmp(operator, "==") == 0) {
        result = register_value == operand_value;
    } else if (strcmp(operator, "!=") == 0) {
        result = register_value != operand_value;
    } else if (strcmp(operator, "<=") == 0) {
        result = register_value <= operand_value;
    } else if (strcmp(operator, ">=") == 0) {
        result = register_value >= operand_value;
    } else if (strcmp(operator, ">") == 0) {
        result = register_value > operand_value;
    } else if (strcmp(operator, "<") == 0) {
        result = register_value < operand_value;
    } else {
        printf("Error: Invalid operator. Line %hd.\n",instr_processor->num_instruction + 1);
    }

    if (result == 0) {
        instr_processor->valid_if = false;
        while (1) {
            instr_processor->num_instruction++;
            instr_processor->instruction = instruction_fetch(cpu, program, index_core);

            instr_processor->type = instruction_decode(instr_processor->instruction, instr_processor->num_instruction);

            instruction_copy = strdup(instr_processor->instruction);

            token = strtok(instruction_copy, " "); 
            trim(token);

            if (strcmp(token, "I_END") == 0)
                break;
        }
    } else {
        instr_processor->valid_if = true;
        instr_processor->running_if = true;
    }

    free(instruction_copy);
}

void if_end(instruction_processor* instr_processor) {
    char *instruction_copy, *token;

    instruction_copy = strdup(instr_processor->instruction);

    token = strtok(instruction_copy, " "); 
    trim(token);

    if (strcmp(token, "I_END") != 0) {
        printf("Error: Invalid instruction - I_END\n");
        exit(1);
    }

    instr_processor->running_if = false;
}

void else_i(cpu* cpu, char* program, instruction_processor* instr_processor, unsigned short int index_core) {
    char* instruction_copy, *token;
    
    instruction_copy = strdup(instr_processor->instruction);

    token = strtok(instruction_copy, " ");
    trim(token);

    if (strcmp(token, "ELSE") != 0) {
        printf("Error: Invalid instruction - ELSE\n");
        exit(1);
    }

    if (instr_processor->has_if && !instr_processor->valid_if) {
        instr_processor->has_if = false;
    }
    else if (instr_processor->running_if) {
        printf("Error: Invalid instruction\n");
        exit(1);
    }
    else if (!instr_processor->has_if) {
        printf("Error: Invalid instruction. No IF before ELSE. Line %hd.\n",instr_processor->num_instruction + 1);
        exit(1);
    }
    else if (instr_processor->has_if && instr_processor->valid_if) {
        while (1) {
            instr_processor->num_instruction++;
            instr_processor->instruction = instruction_fetch(cpu, program, index_core);

            instr_processor->type = instruction_decode(instr_processor->instruction, instr_processor->num_instruction);
            instruction_copy = strdup(instr_processor->instruction);
            token = strtok(instruction_copy, " "); 

            if (strcmp(token, "ELS_END") == 0)
                break;
        }
    }
    
    free(instruction_copy);
}

void else_end(instruction_processor* instr_processor) {
    char* instruction_copy, *token;
    
    instruction_copy = strdup(instr_processor->instruction);
    token = strtok(instruction_copy, " ");
    trim(token);

    if (strcmp(token, "ELS_END") != 0) {
        printf("Error: Invalid instruction - ELS_END\n");
        exit(1);
    }

    instr_processor->has_if = false;
}

void loop(cpu* cpu, instruction_processor* instr_processor, unsigned short int index_core) {
    char *instruction_copy, *token, *register_name;
    unsigned short int value, register_index;

    instruction_copy = strdup(instr_processor->instruction);

    token = strtok(instruction_copy, " "); 
    trim(token);

    if (strcmp(token, "LOOP") != 0) {
        printf("Error: Invalid instruction - LOOP\n");
        exit(1);
    }

    if (!instr_processor->loop) {
        token = strtok(NULL, " ");
        trim(token);
        if (isdigit(token[0])) {
            value = atoi(token);
            if (value == 0) {
                printf("Error: Loop value can't be 0. Line %hd.\n",instr_processor->num_instruction + 1);
                exit(1);
            }
        } else {
            register_name = token;
            
            trim(register_name);
            register_index = get_register_index(register_name);

            value = cpu->core[index_core].registers[register_index];
            if (value == 0) {
                printf("Error: Loop value can't be 0. Line %hd.\n",instr_processor->num_instruction + 1);
                exit(1);
            }
        }
        instr_processor->loop_value = value;
        instr_processor->loop_start = instr_processor->num_instruction;
        instr_processor->loop = true;
    }
}

void loop_end(cpu* cpu, instruction_processor* instr_processor, unsigned short int index_core) {
    char *instruction_copy, *token;

    instruction_copy = strdup(instr_processor->instruction);

    token = strtok(instruction_copy, " "); 
    trim(token);

    if (strcmp(token, "L_END") != 0) {
        printf("Error: Invalid instruction - L_END\n");
        exit(1);
    }

    int decrease = instr_processor->num_instruction - instr_processor->loop_start + 1;
    instr_processor->loop_value--;
    if (instr_processor->loop_value == 0) {
        instr_processor->loop = false;
        instr_processor->loop_start = 0;
        instr_processor->num_instruction++;
    }
    else {
        for (int i=0; i<decrease; i++) {
            cpu->core[index_core].PC--;
        }
        instr_processor->num_instruction = instr_processor->loop_start;
    }
}