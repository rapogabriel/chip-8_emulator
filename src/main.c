
#include <SDL3/SDL.h>
#include "core.h"

int main(void){
    App app = {0};

    if (appInit(&app, "Chip-8 Emulator", 800, 600)){
        appRun(&app);
        appDestroy(&app);
    }

    // SDL_Surface* surface_icone = SDL_LoadPNG("../imgs/close_icon.png");
    // if (!surface_icone) goto Erro_apos_window;

    // SDL_SetSurfaceColorKey(surface_icone, true, SDL_MapRGB(SDL_GetPixelFormatDetails(surface_icone->format), NULL, 255, 255, 255));

    // SDL_Texture* textura_icone = SDL_CreateTextureFromSurface(renderer, surface_icone);
    // if (!textura_icone) goto Erro_apos_window;
    // SDL_DestroySurface(surface_icone);

    // SDL_FRect local_icone = { 
    //     .x = 800.0f - 16.0f - 10.0f, // 800 - largura_icone - margem_direita
    //     .y = (36.0f - 16.0f) / 2.0f, // (altura_barra - altura_icone) / 2 (para centralizar)
    //     .w = 16.0f,
    //     .h = 16.0f
    // };
    // SDL_FRect bar = { 0.0f, 0.0f, 800.0f, 36.0f };
    // bool rodando = true;
    // SDL_Event evento;

    // SDL_SetWindowHitTest(window, regras_da_janela, nullptr);

    // while (rodando) {
    
    //     while (SDL_PollEvent(&evento)) {
    //         if(evento.type == SDL_EVENT_QUIT){
    //             rodando = false;
    //         }
    //     }

    //     SDL_SetRenderDrawColor(renderer, 20, 20, 50, 255);
    //     SDL_RenderClear(renderer);
    //     SDL_SetRenderDrawColor(renderer, 5, 5, 5, 255);
    //     SDL_RenderFillRect(renderer, &bar);
    //     for(int i = 0; i < QTD_BTN; ++i){
    //         SDL_RenderTexture(renderer, textura_icone, NULL, &local_icone);
    //     }
    //     SDL_RenderPresent(renderer);
    // }

    // SDL_DestroyRenderer(renderer);
    // SDL_DestroyWindow(window);
    // SDL_Quit();

    // return 0;

    // Erro_apos_window:
    // printf("Erro: %s\n", SDL_GetError());
    // SDL_DestroyRenderer(renderer);
    // SDL_DestroyWindow(window);
    // Erro_antes_window:
    // SDL_Quit();
    // return 1;
}