#include "CPU.h"

void control_unit(si opcode) {
    switch(opcode) {
    case 0: // ADD
    // Instruções para soma
    break;
    case 1: // SUB
    // Instruções para subtração
    break;
    case 2: // AND
    // Instruções para AND
    break;
    case 3: // OR
    // Instruções para OR
    break;
    case 4: // XOR
    // Instruções para XOR
    break;
    case 5: // SLL
    // Instruções para SLL
    break;
    case 6: // SRL
    // Instruções para SRL
    break;
    case 7: // JUMP
    // Instruções para JUMP
    break;
    case 8: //LOAD
    // Instruções para LOAD
    break;
    case 9: //STORE
    // Instruções para STORE
    break;
    default:
    // Instruções para instruções não reconhecidas 
    break;
    }
}

si ula_operation(CPU cpu, si a, si b, si operation, RAM memoria) {
    switch(operation) {
        case 0: // ADD
        return a + b;
        break;
        case 1: // SUB
        return a - b;
        break;
        case 2: // AND
        return a & b;
        break;
        case 3: // OR
        return a | b;
        break;
        case 4: // XOR
        return a ^ b;
        break;
        case 5: // SLL
        return a << b;
        break;
        case 6: // SRL
        return a >> b;
        break;
        case 7: // JUMP
        jump(cpu, 2);
        break;
        case 8: //LOAD
        load(cpu, 2, 3, 4, memoria);
        break;
        case 9: //STORE
        store(cpu, 2, 3, 4, memoria);
        break;
        default:
        return 0; 
        break;
    }
}

void jump (CPU cpu, si address) {
    cpu.PC = address;
}

void load(CPU cpu, si registrador_destino, si offset, si registrador_base, RAM memoria) {
    si endereco = cpu.registers[registrador_base] + offset;
    
    cpu.registers[registrador_destino] = memoria.vector[endereco];
}

void store(CPU cpu, si registrador_fonte, si offset, si registrador_base, RAM memoria) {
    si endereco = cpu.registers[registrador_base] + offset;

    memoria.vector[endereco] = cpu.registers[registrador_fonte]; //memoria
}
