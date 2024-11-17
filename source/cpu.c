#include "cpu.h"
#include "gb.h"

#include <stdint.h>
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
/* LD X, Y */
#define LD_X_Y(x, y) \ 
static void ld_##x##_##y(GB_gameboy_t *gb, uint8_t opcode) \
{ \
    gb->x = gb->y; \
}

/* LD X, [HL] */
#define LD_X_DHL(x) \
static void ld_##x##_##dhl(GB_gameboy_t *gb, uint8_t opcode) \
{ \
    gb->x = GB_read(gb, gb->hl);\
}

/* LD [HL], R8*/
#define LD_DHL_Y(y) \
static void ld_##dhl##_##y(GB_gameboy_t *gb, uint8_t opcode) \ 
{ \
    GB_write(gb, gb->hl, gb->y); \ 
}

/* INC R16*/
#define INC_XX(xx) \ 
static void inc_##xx(GB_gameboy_t *gb, uint8_t opcode) \ 
{ \
    gb->xx++; \ 
}

/* DEC R16*/
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

/* all other instructions are hand written */
static void nop(GB_gameboy_t *gb, uint8_t opcode) {}

/* INC R8*/
// inc_hr for instructions operating on higher 8-bit of the 16-bit regs
// affects: ZERO, HALF_CARRY, SUBTRACT_FLAG
static void inc_hr(GB_gameboy_t* gb, uint8_t opcode) {
    // select the 16 bit reg and get the right value
    uint8_t reg_id = ((opcode & 0xF0) >> 4) + 1;
    uint8_t value = (gb->registers[reg_id] >> 8); 

    // reset affected flags
    gb->f &= ~(GB_HALF_CARRY_FLAG | GB_ZERO_FLAG | GB_SUBTRACT_FLAG);

    // check flags
    if((value & 0x0F) == 0x0F) gb->f |= GB_HALF_CARRY_FLAG;
    value++;
    if(value == 0x00) gb->f |= GB_ZERO_FLAG;

    // clean hi bits
    gb->registers[reg_id] &= 0x00FF;
    // set hi bits
    gb->registers[reg_id] |= (0xFF00 & (value << 8));
}
// inc_lr for instructions operating on lower 8-bits of the 16-bit regs
static void inc_lr(GB_gameboy_t* gb, uint8_t opcode){
    uint8_t reg_id = ((opcode & 0xF0) >> 4) + 1;
    uint8_t value = gb->registers[reg_id];

    gb->f &= ~(GB_HALF_CARRY_FLAG | GB_ZERO_FLAG | GB_SUBTRACT_FLAG);

    if((value & 0x0F) == 0x0F) gb->f |= GB_HALF_CARRY_FLAG;
    value++;
    if(value == 0x00) gb->f |= GB_ZERO_FLAG;

    gb->registers[reg_id] &= 0xFF00;
    gb->registers[reg_id] |= (0x00FF & value) ;
}
// INC A has it's own function
static void inc_a(GB_gameboy_t* gb, uint8_t opcode) {
    uint8_t value = gb->a;

    gb->f &= ~(GB_HALF_CARRY_FLAG | GB_ZERO_FLAG | GB_SUBTRACT_FLAG);
    
    if((value & 0x0F) == 0x0F) gb->f |= GB_HALF_CARRY_FLAG;
    value++;
    if(value == 0x00) gb->f |= GB_ZERO_FLAG;

    gb->a = value;
}

/* DEC R8 */
// similar to INC R8f
// affects: ZERO, HALF_CARRY, SUBTRACT
static void dec_hr(GB_gameboy_t* gb, uint8_t opcode) {
    uint8_t reg_id = ((opcode & 0xF0) >> 4) + 1;
    uint8_t value = gb->registers[reg_id] >> 8;

    gb->f &= ~(GB_HALF_CARRY_FLAG | GB_ZERO_FLAG);
    gb->f |= GB_SUBTRACT_FLAG;

    if((value & 0x0F) == 0x00) gb->f |= GB_HALF_CARRY_FLAG;
    value--;
    if(value == 0x00) gb->f |= GB_ZERO_FLAG;

    gb->registers[reg_id] &= 0x00FF;
    gb->registers[reg_id] |= ((value << 8) & 0xFF00);
}
static void dec_lr(GB_gameboy_t* gb, uint8_t opcode){
    uint8_t reg_id = ((opcode & 0xF0) >> 4) + 1;
    uint8_t value = gb->registers[reg_id];

    gb->f &= ~(GB_HALF_CARRY_FLAG | GB_ZERO_FLAG);
    gb->f |= GB_SUBTRACT_FLAG;

    if((value & 0x0F) == 0x00) gb->f |= GB_HALF_CARRY_FLAG;
    value--;
    if(value == 0x00) gb->f |= GB_ZERO_FLAG;

    gb->registers[reg_id] &= 0xFF00;
    gb->registers[reg_id] |= (0x00FF & value);
}
static void dec_a(GB_gameboy_t* gb, uint8_t opcode) {
    uint8_t value = gb->a;

    gb->f &= ~(GB_HALF_CARRY_FLAG | GB_ZERO_FLAG);
    gb->f |= GB_SUBTRACT_FLAG;

    if((value & 0x0F) == 0x00) gb->f |= GB_HALF_CARRY_FLAG;
    value--;
    if(value == 0x00) gb->f |= GB_ZERO_FLAG;

    gb->a = value;
}

/* ARITHMETIC OPERATIONS */
uint8_t get_register_value(GB_gameboy_t* gb, uint8_t opcode) {
    // opcodes ending with 7 or F use the Accumulator
    if (((opcode & 0x0F) == 0x07) || 
        ((opcode & 0x0F) == 0x0F)) return gb->a;

    // select 16 bit register from the array
    uint8_t reg_id = 1 + ((opcode & 0x02) >> 1) + ((opcode & 0x04) >> 1);
    // select high or low?
    int apply_shift = (opcode & 0x1) == 0x0;

    // return
    return gb->registers[reg_id] >> 8 * apply_shift;
} 

/* ADD A,R8 & ADC A,R8*/
// affected flags: ZERO, SUBTRACT, HALF_CARRY, CARRY
static void add_r(GB_gameboy_t* gb, uint8_t opcode) {
    uint8_t value = get_register_value(gb, opcode);
    uint8_t carry = 0;

    // check if instruction uses carry
    if((opcode & 0x08) == 0x08) carry = (gb->f & GB_CARRY_FLAG) != 0;

    // clean flags, since i have to update all of them!
    // if condition for a flag it is set, otherwise it is unset
    gb->f &= ~(GB_ZERO_FLAG | GB_SUBTRACT_FLAG | GB_HALF_CARRY_FLAG | GB_CARRY_FLAG);

    // check half carry
    if(((gb->a & 0x0F) + (value & 0x0F) + carry) > 0x0F) 
        gb->f |= GB_HALF_CARRY_FLAG;
    // check carry
    if((gb->a + value + carry) > 0xFF) 
        gb->f |= GB_CARRY_FLAG;

    // do the math!
    gb->a += value + carry ;

    // check if result is zero
    if(gb->a == 0x00) gb->f |= GB_ZERO_FLAG;
}
/* SUB A,R8 & SBC A,R8*/
static void sub_r(GB_gameboy_t* gb, uint8_t opcode) {
    /* similar to add_r */
    uint8_t value = get_register_value(gb, opcode);
    uint8_t carry = 0;

    if((opcode  & 0x08) == 0x08) carry = ((gb->f & GB_CARRY_FLAG) != 0);

    // always clean flags! same as add_r
    gb->f &= ~(GB_ZERO_FLAG | GB_HALF_CARRY_FLAG | GB_CARRY_FLAG);
    gb->f &= GB_SUBTRACT_FLAG;


    if( ((value - carry) & 0x0F) > (gb->a & 0x0F) ) gb->f |= GB_HALF_CARRY_FLAG;
    if( (value - carry) > gb->a ) gb->f |= GB_CARRY_FLAG;

    gb->a -= value - carry;

    if(gb->a == 0x00) gb->f |= GB_ZERO_FLAG;
}
/* AND A,R8*/
static void and_r(GB_gameboy_t* gb, uint8_t opcode) {
    uint8_t value = get_register_value(gb, opcode);

    gb->a &= value;
    gb->f &= ~(GB_ZERO_FLAG | GB_SUBTRACT_FLAG | GB_CARRY_FLAG);
    gb->f |= GB_HALF_CARRY_FLAG;

    // load `value` in register a, and fill with 0x00 register f

    if (gb->a == 0x00) gb->f |= GB_ZERO_FLAG;
}
/* XOR A,R8*/
static void xor_r(GB_gameboy_t* gb, uint8_t opcode) {
    uint8_t value = get_register_value(gb, opcode);

    gb->a ^= value;
    gb->f &= ~(GB_SUBTRACT_FLAG | GB_CARRY_FLAG | GB_HALF_CARRY_FLAG | GB_ZERO_FLAG);

    if (gb->a == 0x00) gb->f |= GB_ZERO_FLAG; 
}
/* OR  A, R8*/
static void or_r(GB_gameboy_t* gb, uint8_t opcode) {
    uint8_t value = get_register_value(gb, opcode);

    gb->a |= value;
    gb->f &= ~(GB_SUBTRACT_FLAG | GB_CARRY_FLAG | GB_HALF_CARRY_FLAG | GB_ZERO_FLAG);

    if (gb->a == 0x00) gb->f |= GB_ZERO_FLAG;
}
/* CP  A, R8*/
static void cp_r(GB_gameboy_t* gb, uint8_t opcode) {
    uint8_t value = get_register_value(gb, opcode);
    uint8_t a = gb->a;
    gb->f &= ~(GB_ZERO_FLAG | GB_HALF_CARRY_FLAG | GB_CARRY_FLAG);

    if ( a == value )
        gb->f |= GB_ZERO_FLAG;
    if((a & 0x0F) < (value & 0x0F)) 
        gb->f |= GB_HALF_CARRY_FLAG;

    if(a < value) 
        gb->f |= GB_CARRY_FLAG;
}

/* ADD HL, R16*/
static void add_hl_rr(GB_gameboy_t* gb, uint8_t opcode) {

    // zero flag is not modified
    // unset subtract flag
    gb->f &= ~(GB_SUBTRACT_FLAG | GB_HALF_CARRY_FLAG | GB_CARRY_FLAG);

    // get register to operate with (opcode & 0xF0) is actually usless
    uint8_t reg_id = 1 + ((opcode & 0xF0) >> 4);
    uint8_t value = gb->registers[reg_id];

    // check overflow from bit 11
    if(((gb->hl & 0x0FFF) + (value & 0x0FFF)) > 0x0FFF )
        gb->f |= GB_HALF_CARRY_FLAG;
    // check overflow from bit 15
    if((gb->hl + value) > 0xFFFF)
        gb->f |= GB_CARRY_FLAG;

    // do the math
    gb->hl += value;
}

/* arithmetic/logical [HL] instructions */
/* INC [HL] */
static void inc_dhl(GB_gameboy_t *gb, uint8_t opcode){
    uint8_t value = GB_read(gb, gb->hl);

    // same as INC R8 
    gb->f &= ~(GB_HALF_CARRY_FLAG | GB_ZERO_FLAG | GB_SUBTRACT_FLAG);

    if((value & 0x0F) == 0x0F) gb->f |= GB_HALF_CARRY_FLAG;
    value++;
    if(value == 0x00) gb->f |= GB_ZERO_FLAG;

    GB_write(gb, gb->hl, value+1);
}
/* DEC [HL] */
static void dec_dhl(GB_gameboy_t *gb, uint8_t opcode){

    uint8_t value = GB_read(gb, gb->hl);

    // same as DEC R8
    gb->f &= ~(GB_HALF_CARRY_FLAG | GB_ZERO_FLAG);
    gb->f |= GB_SUBTRACT_FLAG;

    if((value & 0x0F) == 0x00) gb->f |= GB_HALF_CARRY_FLAG;
    value--;
    if(value == 0x00) gb->f |= GB_ZERO_FLAG;

    GB_write(gb, gb->hl, value-1);
}
/* ADD A,[HL] & ADC A,[HL]*/
static void add_a_dhl(GB_gameboy_t *gb, uint8_t opcode) {
    uint8_t value = GB_read(gb, gb->hl);
    uint8_t carry = 0;

    if((opcode & 0x08) == 0x08) carry = (gb->f & GB_CARRY_FLAG) != 0;

    gb->f &= ~(GB_ZERO_FLAG | GB_SUBTRACT_FLAG | GB_HALF_CARRY_FLAG | GB_CARRY_FLAG);

    if(((gb->a & 0x0F) + (value & 0x0F) + carry) > 0x0F) 
        gb->f |= GB_HALF_CARRY_FLAG;
    if((gb->a + value + carry) > 0xFF) 
        gb->f |= GB_CARRY_FLAG;

    gb->a += value + carry ;

    if(gb->a == 0x00) gb->f |= GB_ZERO_FLAG;
}
/* SUB A,[HL] & SBC A,[HL]*/
static void sub_a_dhl(GB_gameboy_t *gb, uint8_t opcode) {
    uint8_t value = GB_read(gb, gb->hl);
    uint8_t carry = 0;

    if((opcode  & 0x08) == 0x08) carry = ((gb->f & GB_CARRY_FLAG) != 0);

    gb->f &= ~(GB_ZERO_FLAG | GB_HALF_CARRY_FLAG | GB_CARRY_FLAG);
    gb->f &= GB_SUBTRACT_FLAG;

    if( ((value - carry) & 0x0F) > (gb->a & 0x0F) ) gb->f |= GB_HALF_CARRY_FLAG;
    if( (value - carry) > gb->a ) gb->f |= GB_CARRY_FLAG;

    gb->a -= value - carry;

    if(gb->a == 0x00) gb->f |= GB_ZERO_FLAG;
}
/* AND A,[HL]*/
static void and_a_dhl(GB_gameboy_t *gb, uint8_t opcode) {
    uint8_t value = GB_read(gb, gb->hl);

    // load `value` in register a, and fill with 0x00 register f
    gb->a &= value;
    gb->f &= ~(GB_ZERO_FLAG | GB_SUBTRACT_FLAG | GB_CARRY_FLAG);
    gb->f |= GB_HALF_CARRY_FLAG;

    if (gb->a == 0x00) gb->f |= GB_ZERO_FLAG;
}
/* OR A,[HL]*/
static void or_a_dhl(GB_gameboy_t *gb, uint8_t opcode) {
    uint8_t value = GB_read(gb, gb->hl);

    gb->a |= value;
    gb->f &= ~(GB_SUBTRACT_FLAG | GB_CARRY_FLAG | GB_HALF_CARRY_FLAG | GB_ZERO_FLAG);

    if (gb->a == 0x00) gb->f |= GB_ZERO_FLAG;
}
/* XOR A,[HL]*/
static void xor_a_dhl(GB_gameboy_t *gb, uint8_t opcode) {
    uint8_t value = GB_read(gb, gb->hl);

    gb->a ^= value;
    gb->f &= ~(GB_SUBTRACT_FLAG | GB_CARRY_FLAG | GB_HALF_CARRY_FLAG | GB_ZERO_FLAG);

    if (gb->a == 0x00) gb->f |= GB_ZERO_FLAG; 
}
/* CP A,[HL]*/
static void cp_a_dhl(GB_gameboy_t *gb, uint8_t opcode) {
    uint8_t value = GB_read(gb, gb->hl);
    uint8_t a = gb->a;
    gb->f &= ~(GB_ZERO_FLAG | GB_HALF_CARRY_FLAG | GB_CARRY_FLAG);
 
    if ( a == value )
        gb->f |= GB_ZERO_FLAG;
    if((a & 0x0F) < (value & 0x0F)) 
        gb->f |= GB_HALF_CARRY_FLAG;

    if(a < value) 
        gb->f |= GB_CARRY_FLAG;
}

/* LD instructions */
/* LD R16,D16*/
static void ld_rr_d16(GB_gameboy_t *gb, uint8_t opcode) {
    uint8_t reg_id = (opcode >> 4) + 1;
    // data to load in r16 is stored after pc
    gb->registers[reg_id] =  GB_read_u16(gb, gb->pc+1);
    gb->pc+=2;
}

static void ld_drr_a(GB_gameboy_t *gb, uint8_t opcode) {
    uint8_t reg_id = ((opcode > 4) + 1);
    uint8_t addr = gb->registers[reg_id];
    GB_write(gb, gb->a, addr);
}

static void ld_dhli_a(GB_gameboy_t *gb, uint8_t opcode) {
    GB_write(gb, gb->a, gb->hl);
    gb->hl++;
}

static void ld_dhld_a(GB_gameboy_t *gb, uint8_t opcode) {
    GB_write(gb, gb->a, gb->hl);
    gb->hl--;
}

static void ld_r_d8(GB_gameboy_t *gb, uint8_t opcode) {}
static void ld_a_drr(GB_gameboy_t *gb, uint8_t opcode) {}
static void ld_dd16_a(GB_gameboy_t *gb, uint8_t opcode) {}
static void ld_a_dd16(GB_gameboy_t *gb, uint8_t opcode) {}
static void ld_dd16_sp(GB_gameboy_t *gb, uint8_t opcode) {}


static void halt(GB_gameboy_t *gb, uint8_t opcode) { /* TODO: implement*/}

static opcode_t *
opcodes[256] = {
//  0x0         0x1         0x2        0x3       0x4       0x5       0x6        0x7
//  0x8         0x9         0xA        0xB       0xC       0xD       0xE        0xF
    nop,        ld_rr_d16,  ld_drr_a,  inc_bc,   inc_hr,    dec_hr,    ld_r_d8,   nop, // 0x0
    ld_dd16_sp, add_hl_rr,  nop,       dec_bc,   inc_lr,    dec_lr,    ld_r_d8,   nop,

    nop,        ld_rr_d16,  ld_drr_a,  inc_de,   inc_hr,    dec_hr,    ld_r_d8,   nop, // 0x1
    nop,        add_hl_rr,  nop,       dec_de,   inc_lr,    dec_lr,    ld_r_d8,   nop,

    nop,        ld_rr_d16,  ld_dhli_a,  inc_hl,   inc_hr,    dec_hr,    ld_r_d8,   nop, // 0x2
    nop,        add_hl_rr,  ld_a_dhl,  dec_hl,   inc_lr,    dec_lr,    ld_r_d8,   nop,
    
    nop,        ld_rr_d16,  ld_dhld_a,  inc_sp,   inc_dhl,  dec_dhl,  nop,       nop, // 0x3
    nop,        add_hl_rr,  ld_a_dhl,  dec_sp,   inc_a,    dec_a,    ld_r_d8,   nop,
    
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

void exec_instr(GB_gameboy_t *gb, uint8_t opcode){
    opcodes[opcode](gb, opcode);
}
