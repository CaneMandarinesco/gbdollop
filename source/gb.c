#include "gb.h"
#include "cpu.h"

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#define RAM_SIZE  0X1000
#define VRAM_SIZE 0X2000
#define MBC_SIZE  0X4000

GB_gameboy_t *GB_init(GB_gameboy_t *gb) {
    gb = malloc(sizeof(GB_gameboy_t));
    // fill with 0 gameboy data area
    // memset(gb, 0, sizeof(GB_gameboy_t));


    // 4kib = 2^2 * 2^10 = 2^12
    // 16^3 = 2^12 ->           0x1000 = 4kib
    //                          0x2000 = 8kib
    // 16kib = 2^4 * 2^10 = 2^14
    // 16^3 = 2^12 -> 2^2 * 2^12 = 2^14 
    //                          0x4000 = 16kib

    gb->ram  = malloc(RAM_SIZE);
    gb->vram = malloc(VRAM_SIZE);
    gb->mbc  = malloc(MBC_SIZE);

    GB_reset(gb);

    return gb;
}

GB_gameboy_t *GB_reset(GB_gameboy_t *gb){
    // Follow DMG0 reset procedure
    gb->af = 0x0100;
    gb->bc = 0xFF13;
    gb->de = 0x00C1;
    gb->hl = 0x8403;
    gb->pc = 0x0100;
    gb->sp = 0xFFFE;

    // fill with 0...
    memset(gb->ram,  0, RAM_SIZE);
    memset(gb->vram, 0, VRAM_SIZE);
    memset(gb->mbc,  0, MBC_SIZE);

    return gb;
}

GB_gameboy_t* GB_cleanup(GB_gameboy_t *gb){
    free(gb->ram);
    free(gb->vram);
    free(gb->mbc);
    free(gb);    

    return NULL; /* or gb? */
}

void GB_run(GB_gameboy_t *gb) {
    // TODO: implement GB_run
}

typedef void write_function_t(GB_gameboy_t* gb, uint16_t addr, uint8_t value); 
static write_function_t *const write_map[] = {
    write_rom, write_rom, write_rom, write_rom, // 0000-3FFF
    write_rom, write_rom, write_rom, write_rom, // 4000-7FFF, switchable bank
    write_vram, write_vram,  // 8000-9FFF
    write_null, write_null,  // A000-BFFF external ram
    write_ram,  write_ram,   // C000-CFFF
    write_fxxx
};

typedef uint8_t read_function_t(GB_gameboy_t *gb, uint16_t adr);
static read_function_t *const read_map[] = {
    read_rom, read_rom, read_rom, read_rom,
    read_rom, read_rom, read_rom, read_rom,
    read_vram, read_vram,
    read_null, read_null,
    read_ram, read_ram,
    read_fxxx
};

/* useful for logging*/
static char* const memory_map_names[] = {
    "ROM", "ROM", "ROM", "ROM",
    "ROM", "ROM", "ROM", "ROM",
    "VRAM", "VRAM",
    "ERAM", "ERAM", /* external ram*/
    "RAM", "RAM",
    "FXXX"
};

void GB_write(GB_gameboy_t *gb, uint16_t addr, uint8_t value){
    write_map[addr >> 12](gb, addr, value);
    GB_log_write(gb, addr, value);
}
void GB_write_u16(GB_gameboy_t *gb, uint16_t addr, uint16_t value) {
    GB_write(gb, addr, (uint8_t)(value & (0x00FF)));
    GB_write(gb, addr+1, (uint8_t)((value & (0xFF00))) >> 8);
}

uint8_t GB_read(GB_gameboy_t *gb, uint16_t addr){
    uint16_t value;
    value = read_map[addr >> 12](gb, addr);
    GB_log_read(gb, addr, value);
    return value;
}
uint16_t GB_read_u16(GB_gameboy_t *gb, uint16_t addr){
    return GB_read(gb, addr) | (GB_read(gb, addr+1) << 8);
}

int GB_load_rom(GB_gameboy_t* gb, const char *path){

    return 0;
}

/* MEMORY IO FUNCTIONS*/
static void write_rom(GB_gameboy_t* gb, uint16_t addr, uint8_t value){
    /* assume that we are running a no MBC cart */
    /* other types of cartridges have different MBC that 
       may allow you to write specific memory areas */
    return;
}

static void write_vram(GB_gameboy_t* gb, uint16_t addr, uint8_t value){
    gb->vram[addr-0x8000] = value;
}

static void write_ram(GB_gameboy_t* gb, uint16_t addr, uint8_t value){
    gb->ram[addr-0xC000] = value;
}

static void write_fxxx(GB_gameboy_t* gb, uint16_t addr, uint8_t value){
    // TODO: implement fxxx memory write
    return;
}

static uint8_t read_rom(GB_gameboy_t* gb, uint16_t addr){
    return gb->mbc[addr]; 
}

static uint8_t read_vram(GB_gameboy_t* gb, uint16_t addr){
    return gb->vram[addr-0x8000];
}

static uint8_t read_ram(GB_gameboy_t* gb, uint16_t addr){
    return gb->ram[addr-0xC000];
}

static uint8_t read_fxxx(GB_gameboy_t* gb, uint16_t addr){
    // TODO: implement fxxx memory read
    return 0;
}

/* CONSOLE LOG */
#define CONSOLE_RESET "\033[0m"
#define CONSOLE_HIGHLIGHT(x) "\033[91m" x CONSOLE_RESET

void GB_log_reg(GB_gameboy_t* gb){
    printf("[reg] GB register status: \n");
    printf("> a: "  CONSOLE_HIGHLIGHT("0x%02x") 
            " f: "  CONSOLE_HIGHLIGHT("0x%02x") 
            " af: " CONSOLE_HIGHLIGHT("0x%04x") "\n", gb->a, gb->f, gb->af);
    printf("> b: "  CONSOLE_HIGHLIGHT("0x%02x") 
            " c: "  CONSOLE_HIGHLIGHT("0x%02x") 
            " bc: " CONSOLE_HIGHLIGHT("0x%04x") "\n", gb->b, gb->c, gb->bc);
    printf("> d: "  CONSOLE_HIGHLIGHT("0x%02x") 
            " e: "  CONSOLE_HIGHLIGHT("0x%02x") 
            " de: " CONSOLE_HIGHLIGHT("0x%04x") "\n", gb->d, gb->e, gb->de);
    printf("> h: "  CONSOLE_HIGHLIGHT("0x%02x") 
            " l: "  CONSOLE_HIGHLIGHT("0x%02x") 
            " hl: " CONSOLE_HIGHLIGHT("0x%04x") "\n", gb->h, gb->l, gb->hl);
    printf("> sp: " CONSOLE_HIGHLIGHT("0x%04x") "\n", gb->sp);
    printf("> pc: " CONSOLE_HIGHLIGHT("0x%04x") "\n", gb->pc);
}

void GB_log_read(GB_gameboy_t* gb, uint16_t addr, uint16_t value) {
    printf("[GB_read] Read addr: " CONSOLE_HIGHLIGHT("0x%04x") 
           ", value: " CONSOLE_HIGHLIGHT("0x%02x") "\n", addr, value);
    printf("> Memory area: " CONSOLE_HIGHLIGHT("%s") "\n", memory_map_names[addr >> 12]);
}

void GB_log_write(GB_gameboy_t* gb, uint16_t addr, uint16_t value){
    printf("[GB_write] Write addr: " CONSOLE_HIGHLIGHT("0x%04x") 
           ", value: " CONSOLE_HIGHLIGHT("0x%02x") "\n", addr, value);
    printf("> Memory area: " CONSOLE_HIGHLIGHT("%s") "\n", memory_map_names[addr >> 12]);
}