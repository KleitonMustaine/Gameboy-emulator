#include <stdio.h>
#include <emu.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>


/* COMPONENTS
    CART
    CPU
    Address Bus
    PPU
    TIMER
*/

static emu_context ctx;

emu_context *emu_get_context(){
    return &ctx;
}

void delay(u32 ms){
    SDL_Delay(ms);
}

int emu_run(int argc, char **argv){
}