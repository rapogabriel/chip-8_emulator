#include <stdio.h>
#include <SDL3/SDL.h>

#include "chip8.h"
#include "config.h"
#include "argparser.h"

constexpr int CHIP8_WIDTH = 64;
constexpr int CHIP8_HEIGHT = 32;
constexpr int WINDOW_SCALE = 15;

int main(int argc, char* argv[]){
    Config cfg = setConfig();
    if(!parse(argc, argv, &cfg)){
        printf("Formato inválido\n");
        return 1;
    }
    if (!SDL_Init(SDL_INIT_VIDEO)) {
        SDL_Log("Falha ao inicializar SDL: %s", SDL_GetError());
        return 1;
    }
    SDL_Window* window = SDL_CreateWindow(
        "Emulador CHIP-8", 
        CHIP8_WIDTH * WINDOW_SCALE, 
        CHIP8_HEIGHT * WINDOW_SCALE, 
        0
    );
    if (!window) {
        SDL_Log("Falha ao criar janela: %s", SDL_GetError());
        SDL_Quit();
        return 1;
    }
    SDL_Renderer* renderer = SDL_CreateRenderer(window, NULL);
    if (!renderer) {
        SDL_Log("Falha ao criar renderizador: %s", SDL_GetError());
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }
    SDL_SetRenderLogicalPresentation(renderer, CHIP8_WIDTH, CHIP8_HEIGHT, SDL_LOGICAL_PRESENTATION_LETTERBOX);
    SDL_Texture* texture = SDL_CreateTexture(
        renderer, 
        SDL_PIXELFORMAT_RGBA8888, 
        SDL_TEXTUREACCESS_STREAMING,
        CHIP8_WIDTH, 
        CHIP8_HEIGHT
    );
    SDL_SetTextureScaleMode(texture, SDL_SCALEMODE_NEAREST);
    uint32_t pixels[CHIP8_WIDTH * CHIP8_HEIGHT];
    bool rodando = true;
    SDL_Event evento;
    Chip8* chip = Chip8_new(&cfg);
    Chip8_load_rom(chip, cfg.filepath);
    while (rodando) {
        // 5. Processamento de Eventos (Botão de fechar, etc)
        // No SDL3, SDL_PollEvent também retorna booleano
        while (SDL_PollEvent(&evento)) {
            if (evento.type == SDL_EVENT_QUIT) {
                rodando = false;
            }
        }

        // 6. Atualiza o estado das teclas para o seu array de 16 estados (0-F)
        const bool* teclas_estado = SDL_GetKeyboardState(NULL);
        // Exemplo: se (teclas_estado[SDL_SCANCODE_1]) { cpu.keypad[0x1] = 1; } etc...
        Chip8_update_keyboard(chip, teclas_estado);
        // --- AQUI VAI O SEU FETCH-DECODE-EXECUTE ---
        for(int i = 0; i < 10; ++i)
            Chip8_cycle(chip);
        Chip8_timers_decrement(chip);

        // 7. Renderização (Se a instrução DXYN ou CLEAR alterou a tela)
        // Pega o seu array interno do CHIP-8 (0 ou 1) e converte para cores RGBA
        if(Chip8_get_draw(chip)){
            getRender(chip, pixels);

            // Atualiza a textura na placa de vídeo
            SDL_UpdateTexture(texture, NULL, pixels, CHIP8_WIDTH * sizeof(uint32_t));

            // Limpa, desenha e apresenta
            SDL_RenderClear(renderer);
            SDL_RenderTexture(renderer, texture, NULL, NULL);
            SDL_RenderPresent(renderer);
        }

        // Delay simples para não torrar a CPU (no futuro você usa um timer de 60Hz)
        SDL_Delay(16); 
    }

    // 8. Limpeza e encerramento
    SDL_DestroyTexture(texture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    Chip8_delete(&chip);
    return 0;
}