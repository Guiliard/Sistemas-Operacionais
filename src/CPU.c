#include "CPU.h"

void print_register() {
    core c;
    c.registers[0] = 0;
    printf("The register 0 is %d\n", c.registers[0]);
}