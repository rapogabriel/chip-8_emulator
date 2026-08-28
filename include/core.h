#pragma once

#include "SDL3/SDL_render.h"
#include "SDL3/SDL_video.h"
#include <SDL3/SDL.h>

typedef struct btn {
    SDL_FRect area;
    SDL_Texture* imagem;
    bool hover;
    bool click;
} Btn;

typedef struct App {
    SDL_Window* window;
    SDL_Renderer* render;
    Btn close;
    bool aberta;
} App;

bool appInit(App*, const char*, int, int);
void appRun(App*);
void appDestroy(App*);