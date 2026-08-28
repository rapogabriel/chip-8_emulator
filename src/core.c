

#include "core.h"
#include "SDL3/SDL_events.h"
#include "log.h"
#include "SDL3/SDL_error.h"
#include "SDL3/SDL_init.h"
#include "SDL3/SDL_render.h"
#include "SDL3/SDL_video.h"
#include "render.h"
#include <stdio.h>

void trataEventos(App*);

bool appInit(App* app, const char* titulo, int largura, int altura){
    if (!SDL_Init(SDL_INIT_VIDEO)) {
        SDL_Log("Falha do SDL_Init: %s", SDL_GetError());
        return false;
    }
    if(!SDL_CreateWindowAndRenderer(titulo, largura, altura,
        SDL_WINDOW_BORDERLESS, &app->window, &app->render)){
        printf("Falha na SDL_CreateWindowAndRenderer: %s", SDL_GetError());
        SDL_Quit();
        return false;
    }
    if(!initBtns(app)) return false;
    app->aberta = true;
    sucesso;
    return true;
}

void appRun(App* app){

    while(app->aberta){
        trataEventos(app);
        drawRender(app);
    }
    sucesso;
}

void appDestroy(App* app){
    SDL_DestroyWindow(app->window);
    SDL_DestroyRenderer(app->render);
    destroyBtns(app);
    SDL_Quit();
    sucesso;
}

void trataEventos(App* app){
    SDL_Event evento;
    while(SDL_PollEvent(&evento)){
        if(evento.type == SDL_EVENT_QUIT) app->aberta = false;
    }
}