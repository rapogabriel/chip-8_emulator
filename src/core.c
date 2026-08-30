

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
#include "btn_actions.h"
#include <stdio.h>

void trataEventos(App*);
SDL_HitTestResult SDLCALL regras_da_janela(SDL_Window *, const SDL_Point *ponto, void *);

bool appInit(App* app, const char* titulo, int largura, int altura){
    if (!SDL_Init(SDL_INIT_VIDEO)) {
        SDL_Log("Falha do SDL_Init: %s", SDL_GetError());
        return false;
    }
    if(!SDL_CreateWindowAndRenderer(titulo, largura, altura,
        SDL_WINDOW_BORDERLESS | SDL_WINDOW_RESIZABLE, &app->window, &app->render)){
        printf("Falha na SDL_CreateWindowAndRenderer: %s", SDL_GetError());
        SDL_Quit();
        return false;
    }
    if(!initBtns(app)) return false;
    app->aberta = true;
    app->maximizado = false;
    SDL_SetWindowMaximumSize(app->window, 1920, 1080);
    SDL_SetWindowMinimumSize(app->window, 800, 600);
    SDL_SetWindowHitTest(app->window, regras_da_janela, nullptr);
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
    SDL_MouseButtonFlags mFlags = SDL_GetMouseState(&mx, &my);
    SDL_FPoint mousePos = {mx, my};
    #define X(nomeVar, x, y, w, h, nome_arquivo) \
    app->nomeVar.hover = SDL_PointInRectFloat(&mousePos, &app->nomeVar.area); \
    app->nomeVar.click = app->nomeVar.hover && ((mFlags & SDL_BUTTON_LMASK) != 0);
    #include "btns.inc"
    #include "menus.inc"
    #undef X
    while(SDL_PollEvent(&evento)){
        if(evento.type == SDL_EVENT_QUIT) app->aberta = false;
        else if(evento.type == SDL_EVENT_MOUSE_BUTTON_DOWN){
            if(evento.button.button == SDL_BUTTON_LEFT){
                SDL_FPoint clickPos = {evento.button.x, evento.button.y};
                #define X(nomeVar, x, y, w, h, nome_arquivo) \
                if(SDL_PointInRectFloat(&clickPos, &app->nomeVar.area)){ \
                    app->nomeVar.click = true; \
                    act_##nomeVar(app); \
                    goto FimClick; \
                }
                #include "btns.inc"
                #undef X
                FimClick:
            }
        } else if(evento.type == SDL_EVENT_WINDOW_RESIZED){
            resizing(app, (float) evento.window.data1);
        }
    }
}

SDL_HitTestResult SDLCALL regras_da_janela(SDL_Window * window, const SDL_Point *ponto, void *) {

    constexpr int borda = 5;

    int w, h;
    SDL_GetWindowSize(window, &w, &h);

    const int wborda = w - borda;
    const int hborda = h - borda;

    if(ponto->x < borda && ponto->y < borda) return SDL_HITTEST_RESIZE_TOPLEFT;
    if(ponto->x > wborda && ponto->y < borda) return SDL_HITTEST_RESIZE_TOPRIGHT;
    if(ponto->x < borda && ponto->y > hborda) return SDL_HITTEST_RESIZE_BOTTOMLEFT;
    if(ponto->x > wborda && ponto->y > hborda) return SDL_HITTEST_RESIZE_BOTTOMRIGHT;

    if(ponto->x < borda) return SDL_HITTEST_RESIZE_LEFT;
    if(ponto->x > wborda) return SDL_HITTEST_RESIZE_RIGHT;

    if(ponto->y < borda) return SDL_HITTEST_RESIZE_TOP;
    if(ponto->y > hborda) return SDL_HITTEST_RESIZE_BOTTOM;
    
    if (ponto->y < 36) {
        // Tenho que configurar as areas dos botões pra isso não ativar depois
        if (ponto->x > 43 && ponto->x < 722) { 
            return SDL_HITTEST_DRAGGABLE;
        }
    }
    
    return SDL_HITTEST_NORMAL;
}