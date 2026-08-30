

#include "render.h"
#include "SDL3/SDL_rect.h"
#include "SDL3/SDL_render.h"
#include "core.h"
#include "log.h"

static SDL_FRect bar = { 0.0f, 0.0f, 800.0f, 36.0f };

#define X(nomeVar, x, y, w, h, arq) \
{ \
surface_icone = SDL_LoadPNG("../imgs/"arq); \
if (!surface_icone) return false; \
SDL_SetSurfaceColorKey(surface_icone, true, SDL_MapRGB(SDL_GetPixelFormatDetails(surface_icone->format), NULL, 255, 255, 255)); \
app->nomeVar.imagem = SDL_CreateTextureFromSurface(app->render, surface_icone); \
if (!app->nomeVar.imagem) return false; \
SDL_DestroySurface(surface_icone); \
app->nomeVar.area = (SDL_FRect){x, y, w, h}; \
app->nomeVar.click = false; \
app->nomeVar.hover = false; \
app->nomeVar.corte = (SDL_FRect) {0.0f, 0.0f, 16.0f, 16.0f}; \
LOG("Botão %s carregado", arq); \
}

bool initBtns(App* app){
    SDL_Surface* surface_icone;
    #include "btns.inc"
    sucesso;
    return true;
}
#undef X


#define X(nomeVar, x, y, w, h, arq) \
{ \
if (app->nomeVar.click) { \
    SDL_SetTextureColorMod(app->nomeVar.imagem, 100, 100, 100); \
} else if (app->nomeVar.hover) { \
    SDL_SetTextureColorMod(app->nomeVar.imagem, 190, 190, 190); \
} else { \
    SDL_SetTextureColorMod(app->nomeVar.imagem, 255, 255, 255); \
} \
SDL_RenderTexture(app->render, app->nomeVar.imagem, &app->nomeVar.corte, &app->nomeVar.area); \
}
bool drawRender(App* app){
    SDL_SetRenderDrawColor(app->render, 20, 20, 20, 255);
    SDL_RenderClear(app->render);
    SDL_SetRenderDrawColor(app->render, 5, 5, 5, 255);
    SDL_RenderFillRect(app->render, &bar);
    #include "btns.inc"
    #undef X
    SDL_RenderPresent(app->render);
    return true;
}

#define X(nomeVar, x, y, w, h, arq) \
SDL_DestroyTexture(app->nomeVar.imagem);
void destroyBtns(App* app){
    #include "btns.inc"
    #undef X
    sucesso;
}

#define X(nomeVar, argx, y, argw, h, arq) \
app->nomeVar.area = (SDL_FRect) { argx + (width - 800.0f), y, argw, h};
void resizing(App* app, float width){
    #include "btns.inc"
    #undef X
    bar.w = width;
}