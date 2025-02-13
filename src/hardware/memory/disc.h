#ifndef DISC_H
#define DISC_H

#define NUM_DISC 4096

#include "../../utils/libs.h"

typedef struct disc {
    unsigned short int **matriz;
} disc;

void init_disc(disc* memory_disc);

#endif