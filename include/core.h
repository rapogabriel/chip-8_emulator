#pragma once

#include "SDL3/SDL_render.h"
#include "SDL3/SDL_video.h"
#include <SDL3/SDL.h>

typedef struct App {
    SDL_Window* window;
    SDL_Renderer* render;
    bool aberta;
} App;

bool appInit(App*, const char*, int, int);
void appRun(App*);
void appDestroy(App*);