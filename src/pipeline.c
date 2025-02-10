#include "pipeline.h"

cache_item* init_c_item() {
    cache_item* c_i = malloc(sizeof(cache_item));

    c_i->is_cached = false;
    c_i->reg_index = 0;
    c_i->result = -1;

    return c_i;
}

void verify_cache_instruction(cpu* cpu, unsigned short int index_core, cache2* cache_table, char* instruction, cache_item* c_i) {
    char *instruction_copy, *token,*register_name1, *register_name2, *type, new_inst[50], *inst;
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
            c_i->reg_index = reg_1;
            reg_1 = cpu->core[index_core].registers[reg_1];
            snprintf(new_inst, sizeof(new_inst), "%s %u %u", type, reg_1, value);
        } else {
            register_name2 = token;

            trim(register_name1);
            trim(register_name2);
            reg_1 = get_register_index(register_name1);
            c_i->reg_index = reg_1;
            reg_1 = cpu->core[index_core].registers[reg_1];
            reg_2 = get_register_index(register_name2);
            reg_2 = cpu->core[index_core].registers[reg_2];
            snprintf(new_inst, sizeof(new_inst), "%s %u %u", type, reg_1, reg_2);
        }

        inst = strdup(new_inst);
        c_i->is_cached = search_cache2(cache_table, inst);
        if (c_i->is_cached)
            c_i->result = get_result_cache(cache_table, inst);
    }

}

char* instruction_fetch(cpu* cpu, char* program, unsigned short int index_core) {
    char* instruction = get_line_of_program(program, cpu->core[index_core].PC);
    cpu->core[index_core].PC++;

    return instruction;
}

type_of_instruction instruction_decode(char* instruction, unsigned short int num_instruction) {
    type_of_instruction type = verify_instruction(instruction, num_instruction);

    if (type == INVALID) {
        exit(1);    
    } else {
        return type;
    }
}

void execute(cpu* cpu, char* program, instruction_processor * instr_processor, unsigned short int index_core, cache2* cache_table) {
    control_unit(cpu, program, instr_processor, index_core, cache_table);
}

void memory_access(cpu* cpu, ram* memory_ram, process_control_block* pcb, type_of_instruction type, char* instruction, unsigned short int index_core) {
    if (type == LOAD) {
        load(cpu, instruction, pcb, index_core);
    } else if (type == STORE) {
        store(cpu, memory_ram, pcb, instruction, index_core);
    } else {
        // do nothing
    }
}

void write_back(cpu* cpu, type_of_instruction type, process_control_block* pcb, char* instruction, unsigned short int result, unsigned short int index_core) {

    char* instruction_copy = strdup(instruction);
    unsigned short int register_index;

    if (type == ADD || type == SUB || type == MUL || type == DIV) {

        strtok(instruction_copy, " "); 

        char* register_name = strtok(NULL, " "); 

        trim(register_name);

        register_index = get_register_index(register_name);

        cpu->core[index_core].registers[register_index] = result;

        pcb->in_p->regs[register_index] = result;

    } else if (type == LOAD || type == STORE || type == LOOP || type == L_END||type == IF||type == I_END || type == ELSE || type == ELS_END) {
        // do nothing
    } else {
        printf("Error: Unrecognized instruction\n");
        exit(1);
    }
}