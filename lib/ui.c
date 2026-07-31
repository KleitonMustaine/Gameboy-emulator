#include <ui.h>
#include <emu.h>
#include <SDL2/SDL.h>

static SDL_Window *sdl_window;
static SDL_Renderer *sdl_renderer;

void ui_init(){
    SDL_CreateWindowAndRenderer(160 * 4, 144 * 4, 0, &sdl_window, &sdl_renderer);
    SDL_SetWindowTitle(sdl_window, "GameBoy Emu");
}

void ui_handle_events(){
    SDL_Event e;

    while (SDL_PollEvent(&e)){
        if (e.type == SDL_QUIT){
            emu_get_context()->running = false;
        }
    }
}
