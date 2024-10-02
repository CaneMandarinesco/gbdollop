#pragma once
#include <stdint.h>

enum {
    GB_REGISTER_AF,
    GB_REGISTER_BC,
    GB_REGISTER_DE,
    GB_REGISTER_HL,
    GB_REGISTER_SP,
    GB_REGISTER_PC,
    GB_REGISTERS_16_BIT /* count */
};

enum {
    GB_IO_JOYP      = 0x00

    // TODO: populate with hardware addresses
};

struct GB_gameboy_s {

    /* registers */
    union {
        uint16_t registers[GB_REGISTERS_16_BIT];
        struct {
            uint16_t af, bc, de,
                     hl, sp, pc;
        };
        struct {
            uint8_t GB_REGISTER_ORDER;
        };
    };

    /* cart info */
    char cart_title[15];

    /* memory */
    uint8_t *ram;
    uint8_t *vram;
};
typedef struct GB_gameboy_s GB_gameboy_t;

GB_gameboy_t *GB_init(GB_gameboy_t *gb);
GB_gameboy_t *GB_reset_internal(GB_gameboy_t *gb);
void GB_run(GB_gameboy_t *gb);


int GB_load_rom(GB_gameboy_t *gb, const char *path);
