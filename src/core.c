

#include "core.h"
#include "SDL3/SDL_events.h"
#include "SDL3/SDL_mouse.h"
#include "SDL3/SDL_rect.h"
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
    float mx, my;
    SDL_MouseButtonFlags mflags = SDL_GetMouseState(&mx, &my);
    SDL_FPoint mousePos = {mx, my};
    bool segurandoClick = (mflags & SDL_BUTTON_LMASK) != 0;
    #define X(nomeVar, x, y, w, h, nome_arquivo) \
    app->nomeVar.hover = SDL_PointInRectFloat(&mousePos, &app->nomeVar.area); \
    app->nomeVar.click = (app->nomeVar.hover && segurandoClick);
    BOTOES(X)
    #undef X
    while(SDL_PollEvent(&evento)){
        if(evento.type == SDL_EVENT_QUIT) app->aberta = false;
        else if(evento.type == SDL_EVENT_MOUSE_BUTTON_DOWN){
            if(evento.button.button == SDL_BUTTON_LEFT){
                SDL_FPoint clickPos = {evento.button.x, evento.button.y};
                #define X(nomeVar, x, y, w, h, nome_arquivo) \
                if(SDL_PointInRectFloat(&clickPos, &app->nomeVar.area)){ \
                    app->nomeVar.click = true; \
                    LOG("Evento click"); \
                    goto FimClick; \
                }
                BOTOES(X)
                #undef X
                FimClick:
            }
        }
    }
}