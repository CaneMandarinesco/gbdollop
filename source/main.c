#include <SDL2/SDL_render.h>
#include <SDL2/SDL_video.h>
#include <stdbool.h>
#include <SDL2/SDL.h>
#include "gb.h"
#include <stdio.h>

SDL_Window* window;
SDL_GLContext* context;
SDL_Renderer* renderer;

GB_gameboy_t *gb;
bool should_close = false;

static void handle_events(void){
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        switch(event.type) {
            case SDL_QUIT:
                should_close = true;
                break;
        }
    }
}

static void run(void){
    while(!should_close){
        handle_events();
        GB_run(gb);
    }
}

int main(int argc, char **argv) {
    SDL_Init(SDL_INIT_EVERYTHING & ~SDL_INIT_AUDIO);
    atexit(SDL_Quit);

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

    window = SDL_CreateWindow("gbdollop",
        SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
        160 * 4, 144 * 4,
        SDL_WINDOW_OPENGL);

    if(window == NULL) {
        // write SDL error into stderr 
        fputs(SDL_GetError(), stderr);
        exit(1);
    }

    SDL_SetWindowMinimumSize(window, 160, 144);

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    context = SDL_GL_CreateContext(window);

    // TODO: separate audio init!
    // GB_audio_init();

    // enable dropfile (drag file into window)
    SDL_EventState(SDL_DROPFILE, SDL_ENABLE);
    SDL_ShowCursor(SDL_DISABLE);

    File* f = open("");
    
    run();

    return 0;
}
