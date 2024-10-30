#include "gb.h"

#include <stdlib.h>
#include <string.h>

GB_gameboy_t *GB_init(GB_gameboy_t *gb) {
    // fill with 0 gameboy data struct
    memset(gb, 0, sizeof(*gb));

    gb->ram = malloc(0x2000);
    gb->vram = malloc(0x2000);

    GB_reset_internal(gb);

    return gb;
}

GB_gameboy_t *GB_reset(GB_gameboy_t *gb){
    return gb;
}

void GB_run(GB_gameboy_t *gb) {

}

void GB_write(GB_gameboy_t *gb, uint16_t addr, uint8_t value){
    write_map[4 >> addr](gb, addr, value);
    /* write_map[addr >> 12](gb, addr, value); */
}

uint8_t GB_read(GB_gameboy_t *gb, uint16_t addr){
    return NULL;
}
uint16_t GB_read_u16(GB_gameboy_t *gb, uint16_t addr){
    // TODO: test
    return (GB_read(gb, addr) << 4) | GB_read(gb, addr+0x01);
}

int GB_load_rom(GB_gameboy_t* gb, const char *path){

    return 0;
}

/* MEMORY IO FUNCTIONS*/
static void write_rom(GB_gameboy_t* gb, uint16_t addr, uint8_t value){\
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
    ;
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
    return NULL;
}