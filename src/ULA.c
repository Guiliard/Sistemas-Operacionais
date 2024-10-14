#include "ULA.h"

int ula_operation(int a, int b, int op) {
    switch(op) {
        case ULA_ADD: return a + b;
        case ULA_SUB: return a - b;
        case ULA_AND: return a & b;
        case ULA_OR:  return a | b;
        case ULA_XOR: return a ^ b;
        case ULA_SLL: return a << b;
        case ULA_SRL: return a >> b;
        default: return 0; 
    }
}