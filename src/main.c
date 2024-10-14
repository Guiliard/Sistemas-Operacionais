#include "CPU.h"

int main() {
    Core cpu;
    init_core(&cpu);
    
    set_register(&cpu, 0, 10);
    set_register(&cpu, 1, 20);
    
    printf("Register 0: %d\n", get_register(&cpu, 0));
    printf("Register 1: %d\n", get_register(&cpu, 1));
    
    print_registers(&cpu);
    
    return 0;
}