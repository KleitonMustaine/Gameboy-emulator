#include <stdio.h>
#include <emu.h>
#include <cart.h>
#include <cpu.h>
#include <instructions.h>
#include <ui.h>
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

void emu_cycles(int cpu_cycles){
    //TODO
}

int emu_run(int argc, char **argv){
    
    char *rom_path = (argc > 1) ? argv[1] : "Roms/Zelda.gb";

    if (!cart_loader(rom_path)) {
        printf("Falha ao carregar a ROM: %s\n", rom_path);
        return -1;
    }
    printf("ROM carregada com sucesso!\n");

    SDL_Init(SDL_INIT_VIDEO);
    TTF_Init();
    ui_init();

    inst_coverage();
    cpu_init();

    ctx.running = true;
    ctx.paused = false;
    ctx.ticks = 0;

    while(ctx.running){
        ui_handle_events();

        if(ctx.paused){
            delay(10);
            continue;
        }
        if(!cpu_step()){
            printf("CPU travou\n");
            return -3;
        }
        ctx.ticks++;
    }
    TTF_Quit();
    SDL_Quit();
    return 0;

    return 0;
}