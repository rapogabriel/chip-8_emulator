#include "SDL3/SDL_events.h"
#include "SDL3/SDL_init.h"
#include "SDL3/SDL_render.h"
#include "SDL3/SDL_video.h"
#include <stdio.h>
#include <SDL3/SDL.h>
#include "sdl_aux.h"

int main(void){
    printf("Olá mundo!\n");

    if (!SDL_Init(SDL_INIT_VIDEO)) {
        SDL_Log("Falha ao inicializar SDL: %s", SDL_GetError());
        return 1;
    }

    SDL_Window* window = nullptr;
    SDL_Renderer* renderer = nullptr;

    if (!SDL_CreateWindowAndRenderer("Chip-8 Emulator", 800, 640, SDL_WINDOW_BORDERLESS, &window, &renderer)) {
        printf("Erro ao criar janela/renderizador: %s\n", SDL_GetError());
        SDL_Quit();
        return 1;
    }

    SDL_Surface* surface_icone = SDL_LoadBMP("../imgs/close_icon.bmp");
    if (!surface_icone) {
        printf("Erro ao carregar o ícone: %s\n", SDL_GetError());
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }
    SDL_SetSurfaceColorKey(surface_icone, true, SDL_MapRGB(SDL_GetPixelFormatDetails(surface_icone->format), NULL, 255, 255, 255));

    SDL_Texture* textura_icone = SDL_CreateTextureFromSurface(renderer, surface_icone);
    if (!textura_icone) {
        printf("Erro ao criar textura do ícone: %s\n", SDL_GetError());
        SDL_DestroySurface(surface_icone);
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }
    SDL_DestroySurface(surface_icone);

    SDL_FRect local_icone = { 
        .x = 800.0f - 24.0f - 10.0f, // 800 - largura_icone - margem_direita
        .y = (40.0f - 24.0f) / 2.0f, // (altura_barra - altura_icone) / 2 (para centralizar)
        .w = 24.0f,
        .h = 24.0f
    };
    SDL_FRect bar = { 0.0f, 0.0f, 800.0f, 40.0f };
    bool rodando = true;
    SDL_Event evento;

    SDL_SetWindowHitTest(window, regras_da_janela, nullptr);

    while (rodando) {
    
        while (SDL_PollEvent(&evento)) {
            if(evento.type == SDL_EVENT_QUIT){
                rodando = false;
            }
        }

        SDL_SetRenderDrawColor(renderer, 20, 20, 50, 255);
        SDL_RenderClear(renderer);
        SDL_SetRenderDrawColor(renderer, 50, 50, 50, 255);
        SDL_RenderFillRect(renderer, &bar);
        SDL_RenderTexture(renderer, textura_icone, NULL, &local_icone);
        SDL_RenderPresent(renderer);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}