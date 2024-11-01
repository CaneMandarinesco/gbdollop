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

#define LD_X_DHL(x) \
static void ld_##x##_##dhl(GB_gameboy_t *gb, uint8_t opcode) \
{ \
    gb->x = GB_read(gb, gb->hl);\
}

#define LD_DHL_Y(y) \
static void ld_##dhl##_##y(GB_gameboy_t *gb, uint8_t opcode) \ 
{ \
    GB_write(gb, gb->hl, gb->y); \ 
}

#define INC_XX(xx) \ 
static void inc_##xx(GB_gameboy_t *gb, uint8_t opcode) \ 
{ \
    gb->xx++; \ 
}

#define DEC_XX(xx) \ 
static void dec_##xx(GB_gameboy_t *gb, uint8_t opcode) \ 
{ \
    gb->xx--; \ 
}

/* define all the "LD r, r" instructions*/
LD_X_Y(b, b) LD_X_Y(b, c) LD_X_Y(b, d) LD_X_Y(b, e) LD_X_Y(b, h) LD_X_Y(b, l) LD_X_Y(b, a) 
LD_X_Y(c, b) LD_X_Y(c, c) LD_X_Y(c, d) LD_X_Y(c, e) LD_X_Y(c, h) LD_X_Y(c, l) LD_X_Y(c, a) 
LD_X_Y(d, b) LD_X_Y(d, c) LD_X_Y(d, d) LD_X_Y(d, e) LD_X_Y(d, h) LD_X_Y(d, l) LD_X_Y(d, a) 
LD_X_Y(e, b) LD_X_Y(e, c) LD_X_Y(e, d) LD_X_Y(e, e) LD_X_Y(e, h) LD_X_Y(e, l) LD_X_Y(e, a) 
LD_X_Y(h, b) LD_X_Y(h, c) LD_X_Y(h, d) LD_X_Y(h, e) LD_X_Y(h, h) LD_X_Y(h, l) LD_X_Y(h, a) 
LD_X_Y(l, b) LD_X_Y(l, c) LD_X_Y(l, d) LD_X_Y(l, e) LD_X_Y(l, h) LD_X_Y(l, l) LD_X_Y(l, a) 
LD_X_Y(a, b) LD_X_Y(a, c) LD_X_Y(a, d) LD_X_Y(a, e) LD_X_Y(a, h) LD_X_Y(a, l) LD_X_Y(a, a) 

/* define all the "LD r, [HL]" and "LD [HL], r" instructions */
LD_X_DHL(b) LD_X_DHL(c) LD_X_DHL(d) LD_X_DHL(e) LD_X_DHL(h) LD_X_DHL(l) LD_X_DHL(a)
LD_DHL_Y(b) LD_DHL_Y(c) LD_DHL_Y(d) LD_DHL_Y(e) LD_DHL_Y(h) LD_DHL_Y(l) LD_DHL_Y(a)

/* define 16 bit inc/dec instructions */
INC_XX(bc) INC_XX(de) INC_XX(hl) INC_XX(sp)
DEC_XX(bc) DEC_XX(de) DEC_XX(hl) DEC_XX(sp)

static void nop(GB_gameboy_t *gb, uint8_t opcode) {}

static void inc_hr(GB_gameboy_t* gb, uint8_t opcode) {
    uint8_t reg_id = ((opcode & 0xF0) >> 4) + 1;
    uint8_t value = (gb->registers[reg_id] >> 4);

    if((value & 0x0F) == 0x0F) gb->f |= GB_HALF_CARRY_FLAG;
    value++;
    if(value == 0x00) gb->f |= GB_ZERO_FLAG;
    gb->f |= GB_SUBTRACT_FLAG;

    gb->registers[reg_id] &= (value << 4) | 0x0F;
}
static void inc_lr(GB_gameboy_t* gb, uint8_t opcode){
    uint8_t reg_id = ((opcode & 0xF0) >> 4) + 1;
    uint8_t value = gb->registers[reg_id];

    if((value & 0x0F) == 0x0F) gb->f |= GB_HALF_CARRY_FLAG;
    value++;
    if(value == 0x00) gb->f |= GB_ZERO_FLAG;
    gb->f |= GB_SUBTRACT_FLAG;

    gb->registers[reg_id] &= value ;
}

static void dec_hr(GB_gameboy_t* gb, uint8_t opcode) {
    uint8_t reg_id = ((opcode & 0xF0) >> 4) + 1;
    uint8_t value = gb->registers[reg_id] >> 4;

    if((value & 0x0F) == 0x00) gb->f |= GB_HALF_CARRY_FLAG;
    value--;
    if(value == 0x00) gb->f |= GB_ZERO_FLAG;
    gb->f |= GB_SUBTRACT_FLAG;

    gb->registers[reg_id] &= (value << 4) | 0x0F;
}
static void dec_lr(GB_gameboy_t* gb, uint8_t opcode){
    uint8_t reg_id = ((opcode & 0xF0) >> 4) + 1;
    uint8_t value = gb->registers[reg_id];

    if((value & 0x0F) == 0x00) gb->f |= GB_HALF_CARRY_FLAG;
    value--;
    if(value == 0x00) gb->f |= GB_ZERO_FLAG;
    gb->f |= GB_SUBTRACT_FLAG;

    gb->registers[reg_id] &= value;
}

static uint8_t get_register_value(GB_gameboy_t* gb, uint8_t opcode) {
    if ((opcode & 0x0F) == 0x07) return gb->a;
    uint8_t reg_id = 1 + ((opcode & 0x02) >> 1) + ((opcode & 0x04) >> 2);
    return gb->registers[reg_id] >> 4 * ~(opcode & 0x1);
} 
static void add_r(GB_gameboy_t* gb, uint8_t opcode) {
    uint8_t value = get_register_value(gb, opcode);
    uint8_t carry = 0;

    if((opcode & 0x08) == 0x08) carry = (gb->f & GB_CARRY_FLAG) != 0;
    if(((gb->a & 0x0F) + (value & 0x0F) + carry) > 0x0F) gb->f |= GB_HALF_CARRY_FLAG;
    if((gb->a + value + carry) > 0xFF) gb->f |= GB_CARRY_FLAG;

    gb->a += value + carry;
    if(gb->a == 0x00) gb->f |= GB_ZERO_FLAG;

    // TODO: CHECK CARRY
}
static void sub_r(GB_gameboy_t* gb, uint8_t opcode) {
    uint8_t value = get_register_value(gb, opcode);
    uint8_t carry = 0;
    if((opcode  & 0x08) == 0x08) carry = ((gb->f & GB_CARRY_FLAG) != 0);

    if( ((value - carry) & 0x0F) > (gb->a & 0x0F) ) gb->f |= GB_HALF_CARRY_FLAG;
    if( (value - carry) > gb->a ) gb->f |= GB_CARRY_FLAG;

    gb->a -= value - carry;

    if(gb->a == 0x00) gb->f |= GB_ZERO_FLAG;

    // TODO: CHECK CARRY
}
static void and_r(GB_gameboy_t* gb, uint8_t opcode) {
    uint8_t value = get_register_value(gb, opcode);
    gb->a &= value;
    if (gb->a == 0x00) gb->f |= GB_ZERO_FLAG;
    gb->f |= GB_HALF_CARRY_FLAG;
    gb->f &= ~GB_SUBTRACT_FLAG;
    gb->f &= ~GB_CARRY_FLAG;
}
static void xor_r(GB_gameboy_t* gb, uint8_t opcode) {
    uint8_t value = get_register_value(gb, opcode);
    gb->a ^= value;
    if (gb->a == 0x00) gb->f |= GB_ZERO_FLAG; 
    gb->f &= ~GB_HALF_CARRY_FLAG; 
    gb->f &= ~GB_SUBTRACT_FLAG; 
    gb->f &= ~GB_CARRY_FLAG; 
}
static void or_r(GB_gameboy_t* gb, uint8_t opcode) {
    uint8_t value = get_register_value(gb, opcode);
    gb->a |= value;
    if (gb->a == 0x00) gb->f |= GB_ZERO_FLAG;
    gb->f &= ~GB_HALF_CARRY_FLAG;
    gb->f &= ~GB_SUBTRACT_FLAG;
    gb->f &= ~GB_CARRY_FLAG;
}
static void cp_r(GB_gameboy_t* gb, uint8_t opcode) {
    uint8_t value = get_register_value(gb, opcode);
    if(((gb->a & 0x0F) - (value & 0x0F)) > 0x0F) gb->f |= GB_HALF_CARRY_FLAG;
    if((gb->a - value) > 0xFF) gb->f |= GB_CARRY_FLAG;
    gb->a -= value;
    if(gb->a == 0x00) gb->f |= GB_ZERO_FLAG;

    // TODO: check carry
}

static void add_hl_rr(GB_gameboy_t* gb, uint8_t opcode) {}

/* LD instructions */
static void ld_rr_d16(GB_gameboy_t *gb, uint8_t opcode) {}
static void ld_drr_a(GB_gameboy_t *gb, uint8_t opcode) {}
static void ld_r_d8(GB_gameboy_t *gb, uint8_t opcode) {}
static void ld_a_drr(GB_gameboy_t *gb, uint8_t opcode) {}
static void ld_dd16_a(GB_gameboy_t *gb, uint8_t opcode) {}
static void ld_a_dd16(GB_gameboy_t *gb, uint8_t opcode) {}
static void ld_dd16_sp(GB_gameboy_t *gb, uint8_t opcode) {}

/* arithmetic/logical [HL] instructions */
static void inc_dhl(GB_gameboy_t *gb, uint8_t opcode){}
static void dec_dhl(GB_gameboy_t *gb, uint8_t opcode){}
static void add_a_dhl(GB_gameboy_t *gb, uint8_t opcode) {}
static void sub_a_dhl(GB_gameboy_t *gb, uint8_t opcode) {}
static void and_a_dhl(GB_gameboy_t *gb, uint8_t opcode) {}
static void or_a_dhl(GB_gameboy_t *gb, uint8_t opcode) {}
static void xor_a_dhl(GB_gameboy_t *gb, uint8_t opcode) {}
static void cp_a_dhl(GB_gameboy_t *gb, uint8_t opcode) {}

static void halt(GB_gameboy_t *gb, uint8_t opcode) {}

static opcode_t *
opcodes[256] = {
//  0x0         0x1         0x2        0x3       0x4       0x5       0x6        0x7
//  0x8         0x9         0xA        0xB       0xC       0xD       0xE        0xF
    nop,        ld_rr_d16,  nop,       inc_bc,   inc_hr,    dec_hr,    ld_r_d8,   nop, // 0x0
    ld_dd16_sp, add_hl_rr,  nop,       dec_bc,   inc_lr,    dec_lr,    ld_r_d8,   nop,

    nop,        ld_rr_d16,  nop,       inc_de,   inc_hr,    dec_hr,    ld_r_d8,   nop, // 0x1
    nop,        add_hl_rr,  nop,       dec_de,   inc_lr,    dec_lr,    ld_r_d8,   nop,

    nop,        ld_rr_d16,  ld_dhl_a,  inc_hl,   inc_hr,    dec_hr,    ld_r_d8,   nop, // 0x2
    nop,        add_hl_rr,  ld_a_dhl,  dec_hl,   inc_lr,    dec_lr,    ld_r_d8,   nop,
    
    nop,        ld_rr_d16,  ld_dhl_a,  inc_sp,   inc_dhl,  dec_dhl,  nop,       nop, // 0x3
    nop,        add_hl_rr,  ld_a_dhl,  dec_sp,   inc_hr,    dec_hr,    ld_r_d8,   nop,
    
    ld_b_b,     ld_b_c,     ld_b_d,    ld_b_e,   ld_b_h,   ld_b_l,   ld_b_dhl,  ld_b_a, // 0x4
    ld_c_b,     ld_c_c,     ld_c_d,    ld_c_e,   ld_c_h,   ld_c_l,   ld_c_dhl,  ld_c_a,
    
    ld_d_b,     ld_d_c,     ld_d_d,    ld_d_e,   ld_d_h,   ld_d_l,   ld_d_dhl,  ld_d_a, // 0x5
    ld_e_b,     ld_e_c,     ld_e_d,    ld_e_e,   ld_e_h,   ld_e_l,   ld_e_dhl,  ld_e_a,
    
    ld_h_b,     ld_h_c,     ld_h_d,    ld_h_e,   ld_h_h,   ld_h_l,   ld_h_dhl,  ld_h_a, // 0x6
    ld_l_b,     ld_l_c,     ld_l_d,    ld_l_e,   ld_l_h,   ld_l_l,   ld_l_dhl,  ld_l_a,
    
    ld_dhl_b,   ld_dhl_c,   ld_dhl_d,  ld_dhl_e, ld_dhl_h, ld_dhl_l, halt,      ld_dhl_a, // 0x7
    ld_a_b,     ld_a_c,     ld_a_d,    ld_a_e,   ld_a_h,   ld_a_l,   ld_a_dhl,  ld_a_a,
    
    add_r,      add_r,      add_r,     add_r,    add_r,    add_r,    add_a_dhl, add_r, // 0x8
    add_r,      add_r,      add_r,     add_r,    add_r,    add_r,    add_a_dhl, add_r,
    
    sub_r,      sub_r,      sub_r,     sub_r,    sub_r,    sub_r,    sub_a_dhl, sub_r, // 0x9
    sub_r,      sub_r,      sub_r,     sub_r,    sub_r,    sub_r,    sub_a_dhl, sub_r,
    
    and_r,      and_r,      and_r,     and_r,    and_r,    and_r,    and_a_dhl, and_r, // 0xA
    xor_r,      xor_r,      xor_r,     xor_r,    xor_r,    xor_r,    xor_a_dhl, xor_r,
    
    or_r,       or_r,       or_r,      or_r,     or_r,     or_r,     or_a_dhl,  or_r, // 0xB
    cp_r,       cp_r,       cp_r,      cp_r,     cp_r,     cp_r,     cp_a_dhl,  cp_r,
    
    nop, nop, nop, nop, nop, nop, nop, nop,  // 0xC
    nop, nop, nop, nop, nop, nop, nop, nop,
    
    nop, nop, nop, nop, nop, nop, nop, nop,  // 0xD
    nop, nop, nop, nop, nop, nop, nop, nop,
    
    nop, nop, nop, nop, nop, nop, nop, nop,  // 0xE
    nop, nop, nop, nop, nop, nop, nop, nop,
    
    nop, nop, nop, nop, nop, nop, nop, nop,  // 0xF
    nop, nop, nop, nop, nop, nop, nop, nop
};

void GB_cpu_run(GB_gameboy_t *gb){ 
    uint8_t opcode = 0;
    opcodes[opcode](gb, opcode);
}