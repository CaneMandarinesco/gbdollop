#include <stdbool.h>
#include <stdio.h>


#include "gb.h"
#include "cpu.h"
#include "test.h"

GB_gameboy_t *gb;
bool should_close = false;

int main(int argc, char **argv) {
    gb = GB_init(gb);

    GB_load_rom(gb, argv[1]);

    GB_run(gb);

    GB_cleanup(gb);
    return 0;
}
