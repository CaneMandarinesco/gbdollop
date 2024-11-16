#include "test.h"

#include <stdio.h>
#include "gb.h"
#include "cpu.h"

void test_arithmetic(void) {
    GB_gameboy_t* gb;
    gb = GB_init(gb);

    printf("Arithmetic test.\n");

    for(int i = 0x80; i < 0xC0; i++) {
        uint8_t v = get_register_value(gb, i);
        printf("\nExectuing opcode: 0x%02x\n");

        exec_instr(gb, i);

        getchar();
    }

    GB_cleanup(gb);
}

void test_inc_r8(void) {
    GB_gameboy_t* gb;
    gb = GB_init(gb);

    printf("INC R8 test\n");
    GB_log_reg(gb);

    // a0 = 1010 0000
    exec_instr(gb, 0x04);
    exec_instr(gb, 0x14);
    exec_instr(gb, 0x24);
    exec_instr(gb, 0x34);

    exec_instr(gb, 0x0C);
    exec_instr(gb, 0x1C);
    exec_instr(gb, 0x2C);
    exec_instr(gb, 0x3C);

    GB_log_reg(gb);

    getchar();

    GB_cleanup(gb);
}

void test_dec_r8(void) {
    GB_gameboy_t* gb;
    gb = GB_init(gb);

    printf("DEC R8 test\n");
    GB_log_reg(gb);
    exec_instr(gb, 0x05);
    exec_instr(gb, 0x15);
    exec_instr(gb, 0x25);
    exec_instr(gb, 0x35);

    exec_instr(gb, 0x0D);
    exec_instr(gb, 0x1D);
    exec_instr(gb, 0x2D);
    exec_instr(gb, 0x3D);
    GB_log_reg(gb);

    getchar();

    GB_cleanup(gb);
}

void test_arithmetic_dhl(void) {
    GB_gameboy_t* gb;
    gb = GB_init(gb);

    gb->ram[0x0000] = 0xFF;
    gb->ram[0x0001] = 0xFF;
    gb->ram[0x0002] = 0xFF;
    gb->ram[0x0003] = 0xFF;

    GB_log_reg(gb);

    gb->hl = 0xC000; // first addr of ram
    exec_instr(gb, 0x34); // inc
    GB_log_reg(gb);

    gb->hl = 0xC001;
    exec_instr(gb, 0x35); // dec
    GB_log_reg(gb);

    gb->hl = 0xC002;
    exec_instr(gb, 0x86); // add
    GB_log_reg(gb);

    gb->hl = 0xC003;
    exec_instr(gb, 0x96); // sub
    GB_log_reg(gb);

    getchar();
    GB_cleanup(gb);
}