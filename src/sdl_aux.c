

#include "sdl_aux.h"

SDL_HitTestResult SDLCALL regras_da_janela([[maybe_unused]]SDL_Window *janela, const SDL_Point *ponto, [[maybe_unused]]void *dados){
    if (ponto->y < 40) {
        return SDL_HITTEST_DRAGGABLE;
    }
    return SDL_HITTEST_NORMAL;
}