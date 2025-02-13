#ifndef PERIPHERALS_H
#define PERIPHERALS_H

#include "../../utils/libs.h"

typedef struct peripherals {
    bool input;
} peripherals;

void init_peripherals(peripherals* peripherals);

#endif