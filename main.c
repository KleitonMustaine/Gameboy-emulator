#include <stdio.h>
#include <SDL2/SDL.h>

#include <cpu.h>
#include <bus.h>
#include <common.h>
#include <emu.h>
#include <ppu.h>
#include <timer.h>
#include <cart.h>

int main(int argc, char **argv){

    char *rom_path = (argc > 1) ? argv[1] : "Roms/dmg-acid2.gb";

    if (!cart_loader(rom_path)) {
        printf("Falha ao carregar a ROM: %s\n", rom_path);
        return -1;
    }

    printf("ROM carregada com sucesso!\n");
    return 0;
}