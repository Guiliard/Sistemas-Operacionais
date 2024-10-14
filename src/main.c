#include "CPU.h"

int main() {
    Core cpu;
    Memory memory;

    init_core(&cpu);
    init_memory(&memory);
    
    // Simulação de alteração dos registradores e memória
    set_register(&cpu, 0, 10);
    set_register(&cpu, 1, 20);
    write_memory(&memory, 0, 1234);
    write_memory(&memory, 4, 5678);

    // Impressão formatada dos registradores e da memória
    print_registers(&cpu);
    print_memory(&memory);
    
    return 0;
}
