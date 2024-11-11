#include "gb.h"
#include "cpu.h"

#include <stdlib.h>
#include <string.h>

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

void GB_write(GB_gameboy_t *gb, uint16_t addr, uint8_t value){
    write_map[4 >> addr](gb, addr, value);
    /* write_map[addr >> 12](gb, addr, value); */
}
void GB_write_u16(GB_gameboy_t *gb, uint16_t addr, uint16_t value) {
    // TODO: implement u16 write
    return;
}

uint8_t GB_read(GB_gameboy_t *gb, uint16_t addr){
    return read_map[4 >> addr](gb, addr);
}
uint16_t GB_read_u16(GB_gameboy_t *gb, uint16_t addr){
    // TODO: test
    return (GB_read(gb, addr) << 4) | GB_read(gb, addr+0x01);
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

void test_arithmetic(void) {
    GB_gameboy_t* gb;
    gb = GB_init(gb);

    printf("Arithmetic test.\n");
    for(int i = 0x80; i < 0xC0; i++) {
        uint8_t v = get_register_value(gb, i);
        printf("\nExectuing opcode: 0x%02x with \n" \
                  "- reg value: %d (0x%02x) \n" \
                  "- a: %d (0x%02x)\n" \
                  "- f: 0x%02x\n", 
                  i , v, v, gb->a, gb->a, gb->f);

        exec_instr(gb, i);

        printf("Results: \n" \
                "- a: %d (0x%02x)\n" \
                "- f: 0x%02x\n",
                gb->a, gb->a, gb->f);

        getchar();
    }

    GB_cleanup(gb);
}