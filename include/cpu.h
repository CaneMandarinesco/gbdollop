#pragma once
#include "gb.h"

// define type of function
typedef void opcode_t(GB_gameboy_t *gb, uint8_t opcode);
void GB_cpu_run(GB_gameboy_t *gb);