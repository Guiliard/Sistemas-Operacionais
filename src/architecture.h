#ifndef ARCHITECTURE_H
#define ARCHITECTURE_H

#include "cpu.h"
#include "ram.h"
#include "disc.h"
#include "peripherals.h"
#include "cache.h"

void init_architecture(cpu* cpu, ram* memory_ram, disc* memory_disc, peripherals* peripherals);

#endif
