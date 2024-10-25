#include "architecture.h"
#include "reader.h"

int main() {
    char *program = read_program("dataset/program.txt");  
    printf("program:\n%s\n", program);

    char *line = get_line_of_program(program, 3);

    printf("line 3: %s\n", line);
    
    free(program);  
    return 0;
}
