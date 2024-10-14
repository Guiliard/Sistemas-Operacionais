#ifndef ULA_H
#define ULA_H

// Operações, depois temos que conferir a funcionaçidade de todas
#define ULA_ADD 0
#define ULA_SUB 1
#define ULA_AND 2
#define ULA_OR  3
#define ULA_XOR 4
#define ULA_SLL 5
#define ULA_SRL 6

int ula_operation(int a, int b, int op);

#endif