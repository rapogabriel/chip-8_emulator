#pragma once

#include "SDL3/SDL_rect.h"
#include "SDL3/SDL_render.h"
#include "SDL3/SDL_video.h"
#include <SDL3/SDL.h>

typedef struct btn {
    SDL_FRect area;
    SDL_Texture* imagem;
    SDL_FRect corte;
    bool hover;
    bool click;
} Btn;

#define X(nomeVar, x, y, w, h, nome_arquivo) \
Btn nomeVar;
typedef struct App {
    SDL_Window* window;
    SDL_Renderer* render;
    #include "btns.inc"
    #include "menus.inc"
    bool aberta;
    bool maximizado;
} App;
#undef X

bool appInit(App*, const char*, int, int);
void appRun(App*);
void appDestroy(App*);