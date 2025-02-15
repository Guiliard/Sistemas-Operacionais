#include "control_unit.h"
#include "../pipeline/pipeline.h"

void control_unit(cpu* cpu, char* program, instruction_processor* instr_processor, unsigned short int index_core, cache* cache_table) {
    instr_processor->result = 0;

    if (instr_processor->type == ADD) {
        instr_processor->result = add(cpu, instr_processor->instruction, index_core, cache_table);
        instr_processor->num_instruction++;
    } else if (instr_processor->type == SUB) {
        instr_processor->result = sub(cpu, instr_processor->instruction, index_core, cache_table);
        instr_processor->num_instruction++;
    } else if (instr_processor->type == MUL) {
        instr_processor->result = mul(cpu, instr_processor->instruction, index_core, cache_table);
        instr_processor->num_instruction++;
    } else if (instr_processor->type == DIV) {
        instr_processor->result = div_c(cpu, instr_processor->instruction, index_core, cache_table);
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

unsigned short int ula(unsigned short int operating_a, unsigned short int operating_b, type_of_instruction operation, cache* cache_table) {
    char new_inst[50], new_inst2[50];
    unsigned short int result;

    if (operation == ADD) {
        result = operating_a + operating_b;
        snprintf(new_inst, sizeof(new_inst), "ADD %u %u", operating_a, operating_b);
        add_cache_instruction(cache_table, new_inst, result);

        snprintf(new_inst2, sizeof(new_inst), "ADD %u %u", operating_b, operating_a);
        add_cache_instruction(cache_table, new_inst2, result);
    }
    else if (operation == SUB) {
        result = operating_a - operating_b;
        snprintf(new_inst, sizeof(new_inst), "SUB %u %u", operating_a, operating_b);
        add_cache_instruction(cache_table, new_inst, result);
    }
    else if (operation == MUL) {
        result = operating_a * operating_b;
        snprintf(new_inst, sizeof(new_inst), "MUL %u %u", operating_a, operating_b);
        add_cache_instruction(cache_table, new_inst, result);

        snprintf(new_inst2, sizeof(new_inst), "MUL %u %u", operating_b, operating_a);
        add_cache_instruction(cache_table, new_inst2, result);
    }
    else {
        result = operating_a / operating_b;
        snprintf(new_inst, sizeof(new_inst), "DIV %u %u", operating_a, operating_b);
        add_cache_instruction(cache_table, new_inst, result);
    }

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
                exit(1); 
            }
            return operating_a / operating_b;

        default:
            printf("Error: Invalid operation.\n");
            exit(1);
    }
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
    exit(1);
}

void verify_cache_instruction(cpu* cpu, unsigned short int index_core, cache* cache_table, char* instruction, instruction_cache_item* inst_cache_item) {
    char *instruction_copy, *token, *register_name1, *register_name2, *type, new_inst[50];
    unsigned short int value, reg_1, reg_2;

    instruction_copy = strdup(instruction);

    token = strtok(instruction_copy, " "); 
    trim(token);

    if (strcmp(token, "ADD") == 0 || strcmp(token, "SUB") == 0 || strcmp(token, "MUL") == 0 || strcmp(token, "DIV") == 0) {
        type = token;
        token = strtok(NULL, " ");
        trim(token);

        register_name1 = token;

        token = strtok(NULL, " ");
        trim(token);

        if (isdigit(token[0])) {
            value = atoi(token);

            trim(register_name1);
            reg_1 = get_register_index(register_name1);
            inst_cache_item->reg_index = reg_1;
            reg_1 = cpu->core[index_core].registers[reg_1];
            snprintf(new_inst, sizeof(new_inst), "%s %u %u", type, reg_1, value);
        } else {
            register_name2 = token;

            trim(register_name1);
            trim(register_name2);
            reg_1 = get_register_index(register_name1);
            inst_cache_item->reg_index = reg_1;
            reg_1 = cpu->core[index_core].registers[reg_1];
            reg_2 = get_register_index(register_name2);
            reg_2 = cpu->core[index_core].registers[reg_2];
            snprintf(new_inst, sizeof(new_inst), "%s %u %u", type, reg_1, reg_2);
        }

        inst_cache_item->is_cached = search_cache_instruction(cache_table, new_inst);
        
        if (inst_cache_item->is_cached)
            inst_cache_item->result = get_result_cache_instruction(cache_table, strdup(new_inst));
    }
}

void add_register_bank_to_pcb(process_control_block *pcb, char *register_name) {
    if (pcb->bank_of_register_used == NULL) {
        size_t length = strlen(register_name) + 3; 
        pcb->bank_of_register_used = (char *)malloc(length);

        if (pcb->bank_of_register_used == NULL) {
            printf("Error: memory allocation failed in bank of register used\n");
            exit(1);
        }

        snprintf(pcb->bank_of_register_used, length, "%s, ", register_name);
    } else {

        size_t bank_length = strlen(pcb->bank_of_register_used);
        char *pattern = (char *)malloc(strlen(register_name) + 3);
        snprintf(pattern, strlen(register_name) + 3, "%s, ", register_name);

        if (strstr(pcb->bank_of_register_used, pattern) == NULL) {
            size_t new_length = bank_length + strlen(register_name) + 3; 
            char *new_memory = (char *)realloc(pcb->bank_of_register_used, new_length);

            if (new_memory == NULL) {
                printf("Error: memory allocation failed in bank of register used\n");
                free(pattern);
                exit(1);
            }

            pcb->bank_of_register_used = new_memory;
            strcat(pcb->bank_of_register_used, register_name);
            strcat(pcb->bank_of_register_used, ", ");
        }

        free(pattern);
    }
}

void add_result_of_process_to_pcb(process_control_block *pcb, char *buffer) {
    if (pcb->result_of_process == NULL) {
        size_t length = strlen(buffer) + 3; 
        pcb->result_of_process = (char *)malloc(length);

        if (pcb->result_of_process == NULL) {
            printf("Error: memory allocation failed in bank of register used\n");
            exit(1);
        }

        snprintf(pcb->result_of_process, length, "%s, ", buffer);
    } else {

        size_t bank_length = strlen(pcb->result_of_process);
        char *pattern = (char *)malloc(strlen(buffer) + 3);
        snprintf(pattern, strlen(buffer) + 3, "%s, ", buffer);

        if (strstr(pcb->result_of_process, pattern) == NULL) {
            size_t new_length = bank_length + strlen(buffer) + 3; 
            char *new_memory = (char *)realloc(pcb->result_of_process, new_length);

            if (new_memory == NULL) {
                printf("Error: memory allocation failed in bank of register used\n");
                free(pattern);
                exit(1);
            }

            pcb->result_of_process = new_memory;
            strcat(pcb->result_of_process, buffer);
            strcat(pcb->result_of_process, ", ");
        }

        free(pattern);
    }
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

    add_register_bank_to_pcb(pcb, register_name);

    cpu->core[index_core].registers[register_index] = value;
    pcb->in_p->regs[register_index] = value;
}

void store (cpu* cpu, ram* memory_ram, process_control_block* pcb, char* instruction, unsigned short int index_core) {
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

    address = verify_address(memory_address, num_positions);

    write_ram(memory_ram, address, buffer);

    add_result_of_process_to_pcb(pcb, buffer);
}

unsigned short int add(cpu* cpu, char* instruction, unsigned short int index_core, cache* cache_table) {
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

        result = ula(cpu->core[index_core].registers[register_index1], value, ADD, cache_table);
    } else {
        register_name2 = token;

        trim(register_name1);
        trim(register_name2);
        register_index1 = get_register_index(register_name1);
        register_index2 = get_register_index(register_name2);

        result = ula(cpu->core[index_core].registers[register_index1], 
                     cpu->core[index_core].registers[register_index2], 
                     ADD, cache_table);
    }

    return result; 
}

unsigned short int sub(cpu* cpu, char* instruction, unsigned short int index_core, cache* cache_table) {
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

        result = ula(cpu->core[index_core].registers[register_index1], value, SUB, cache_table);
    } else {
        register_name2 = token;

        trim(register_name1);
        trim(register_name2);
        register_index1 = get_register_index(register_name1);
        register_index2 = get_register_index(register_name2);

        result = ula(cpu->core[index_core].registers[register_index1], 
                     cpu->core[index_core].registers[register_index2], 
                     SUB, cache_table);
    }

    return result; 
}

unsigned short int mul(cpu* cpu, char* instruction, unsigned short int index_core, cache* cache_table) {
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

        result = ula(cpu->core[index_core].registers[register_index1], value, MUL, cache_table);
    } else {
        register_name2 = token;

        trim(register_name1);
        trim(register_name2);
        register_index1 = get_register_index(register_name1);
        register_index2 = get_register_index(register_name2);

        result = ula(cpu->core[index_core].registers[register_index1], 
                     cpu->core[index_core].registers[register_index2], 
                     MUL, cache_table);
    }

    return result; 
}

unsigned short int div_c(cpu* cpu, char* instruction, unsigned short int index_core, cache* cache_table) {
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

        result = ula(cpu->core[index_core].registers[register_index1], value, DIV, cache_table);
    } else {
        register_name2 = token;

        trim(register_name1);
        trim(register_name2);
        register_index1 = get_register_index(register_name1);
        register_index2 = get_register_index(register_name2);

        result = ula(cpu->core[index_core].registers[register_index1], 
                     cpu->core[index_core].registers[register_index2], 
                     DIV, cache_table);
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
        exit(1);
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