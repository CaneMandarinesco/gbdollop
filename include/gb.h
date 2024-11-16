#pragma once
#include <stdint.h>
#include <stdlib.h>

#define GB_REGISTER_ORDER f, a, \
                          c, b, \
                          e, d, \
                          l, h

enum {
    GB_CARRY_FLAG = 0x10,
    GB_HALF_CARRY_FLAG = 0x20,
    GB_SUBTRACT_FLAG = 0x40,
    GB_ZERO_FLAG = 0x80
}; 

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
    uint8_t *mbc;

    /* memory */
    uint8_t *ram;
    uint8_t *vram;
};
typedef struct GB_gameboy_s GB_gameboy_t;

/* GB INIT, RESET, CLEANUP, RUN*/
GB_gameboy_t *GB_init(GB_gameboy_t *gb);
GB_gameboy_t *GB_reset(GB_gameboy_t *gb);
GB_gameboy_t *GB_cleanup(GB_gameboy_t *gb);
void GB_run(GB_gameboy_t *gb);

/* BUS IO */
void GB_write(GB_gameboy_t *gb, uint16_t addr, uint8_t value);
uint8_t GB_read(GB_gameboy_t *gb, uint16_t addr);
uint16_t GB_read_u16(GB_gameboy_t *gb, uint16_t addr);

/* ROM */
int GB_load_rom(GB_gameboy_t *gb, const char *path);

/* MEMORY IO MAPPING */
static void write_null(GB_gameboy_t* gb, uint16_t addr, uint8_t value) {;}
static void write_rom(GB_gameboy_t* gb, uint16_t addr, uint8_t value);
static void write_vram(GB_gameboy_t* gb, uint16_t addr, uint8_t value);
static void write_ram(GB_gameboy_t* gb, uint16_t addr, uint8_t value);
static void write_fxxx(GB_gameboy_t* gb, uint16_t addr, uint8_t value);

static uint8_t read_null(GB_gameboy_t* gb, uint16_t addr) { return 0; }
static uint8_t read_rom(GB_gameboy_t* gb, uint16_t addr);
static uint8_t read_vram(GB_gameboy_t* gb, uint16_t addr);
static uint8_t read_ram(GB_gameboy_t* gb, uint16_t addr);
static uint8_t read_fxxx(GB_gameboy_t* gb, uint16_t addr);

/* CONSOLE LOG */
void GB_log_reg(GB_gameboy_t* gb);
void GB_log_read(GB_gameboy_t* gb, uint16_t addr, uint16_t value);
void GB_log_write(GB_gameboy_t* gb, uint16_t addr, uint16_t value);