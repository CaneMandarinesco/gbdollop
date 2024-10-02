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

GB_gameboy_t *GB_reset_internal(GB_gameboy_t *gb){
    return gb;
}

void GB_run(GB_gameboy_t *gb) {

}


int GB_load_rom(GB_gameboy_t* gb, const char *path){
    return 0;
}
