#include "cpu.h"

#include <stdlib.h>

/* operand naming conventions
 * r = 8-bit reg
 * rr = 16-bit reg
 * lr = low 8-bit reg
 * hr = high 8-bit reg
 * d8 = 8-bit imm
 * d16 = 16-bit imm
 * d.. = [..]
 * cc = condition code
 */

// NOTE: ## is a concatenation operator
#define LD_X_Y(x, y) \ 
static void ld_##x##_##y(GB_gameboy_t *gb, uint8_t opcode) \
{ \
    gb->x = gb->y; \
}

// TODO: implement read
#define LD_X_DHL(x) \
static void ld_##x##_##dhl(GB_gameboy_t *gb, uint8_t opcode) \
{ \
    gb->x = 0; \ 
}

// TODO: implement write
#define LD_DHL_Y(y) \
static void ld_##dhl##_##y(GB_gameboy_t *gb, uint8_t opcode) \ 
{ \
    ; \ 
}

#define ADD_X
#define SUB_X
#define AND

/* define all the "LD r, r" instructions*/
LD_X_Y(b, b) LD_X_Y(b, c) LD_X_Y(b, d) LD_X_Y(b, e) LD_X_Y(b, h) LD_X_Y(b, l) LD_X_Y(b, a) 
LD_X_Y(c, b) LD_X_Y(c, c) LD_X_Y(c, d) LD_X_Y(c, e) LD_X_Y(c, h) LD_X_Y(c, l) LD_X_Y(c, a) 
LD_X_Y(d, b) LD_X_Y(d, c) LD_X_Y(d, d) LD_X_Y(d, e) LD_X_Y(d, h) LD_X_Y(d, l) LD_X_Y(d, a) 
LD_X_Y(e, b) LD_X_Y(e, c) LD_X_Y(e, d) LD_X_Y(e, e) LD_X_Y(e, h) LD_X_Y(e, l) LD_X_Y(e, a) 
LD_X_Y(h, b) LD_X_Y(h, c) LD_X_Y(h, d) LD_X_Y(h, e) LD_X_Y(h, h) LD_X_Y(h, l) LD_X_Y(h, a) 
LD_X_Y(l, b) LD_X_Y(l, c) LD_X_Y(l, d) LD_X_Y(l, e) LD_X_Y(l, h) LD_X_Y(l, l) LD_X_Y(l, a) 
LD_X_Y(a, b) LD_X_Y(a, c) LD_X_Y(a, d) LD_X_Y(a, e) LD_X_Y(a, h) LD_X_Y(a, l) LD_X_Y(a, a) 

LD_X_DHL()

static opcode_t *opcodes[256];

void GB_cpu_run(GB_gameboy_t *gb){
    uint8_t opcode = 0;
    opcodes[opcode](gb, opcode);
}