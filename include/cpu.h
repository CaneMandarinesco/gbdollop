#pragma once
#include "gb.h"

/* type for cpu instructions */ 
typedef void opcode_t(GB_gameboy_t *gb, uint8_t opcode);

/* get register value from opcode */
uint8_t get_register_value(GB_gameboy_t* gb, uint8_t opcode);

void cpu_run(GB_gameboy_t *gb);

/* execute arbitrary instruction */
void exec_instr(GB_gameboy_t *gb, uint8_t opcode);