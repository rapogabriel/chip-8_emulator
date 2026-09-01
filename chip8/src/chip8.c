#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <string.h>
#include <SDL3/SDL.h>
#include <stddef.h>

#include "chip8.h"
#include "config.h"
#include "debug.h"

// X(campo_chip8, campo_cfg)
#define LISTA(X) \
    X(retro) \
    X(amiga)

constexpr size_t teclas_qtd = 16;

struct _Chip8{
    uint8_t memoria[4096];
    uint8_t registrador[16];
    uint16_t registrador_de_indice;
    uint16_t program_counter;
    uint16_t stack[16];
    uint8_t sp;
    uint8_t delay_timer;
    uint8_t sound_timer;
    uint8_t display[64 * 32];
    bool teclas[teclas_qtd];
    bool retro;
    bool amiga;
    bool draw;
};

static inline void carregarSprites(Chip8* chip){
    const uint8_t chip8_fontset[80] = {
        0xF0, 0x90, 0x90, 0x90, 0xF0, // 0
        0x20, 0x60, 0x20, 0x20, 0x70, // 1
        0xF0, 0x10, 0xF0, 0x80, 0xF0, // 2
        0xF0, 0x10, 0xF0, 0x10, 0xF0, // 3
        0x90, 0x90, 0xF0, 0x10, 0x10, // 4
        0xF0, 0x80, 0xF0, 0x10, 0xF0, // 5
        0xF0, 0x80, 0xF0, 0x90, 0xF0, // 6
        0xF0, 0x10, 0x20, 0x40, 0x40, // 7
        0xF0, 0x90, 0xF0, 0x90, 0xF0, // 8
        0xF0, 0x90, 0xF0, 0x10, 0xF0, // 9
        0xF0, 0x90, 0xF0, 0x90, 0x90, // A
        0xE0, 0x90, 0xE0, 0x90, 0xE0, // B
        0xF0, 0x80, 0x80, 0x80, 0xF0, // C
        0xE0, 0x90, 0x90, 0x90, 0xE0, // D
        0xF0, 0x80, 0xF0, 0x80, 0xF0, // E
        0xF0, 0x80, 0xF0, 0x80, 0x80  // F
    };
    for(int i = 0; i < 80; ++i)
        chip->memoria[0x50 + i] = chip8_fontset[i];
}

#define X(campo) chip->campo = cfg->campo;
Chip8* Chip8_new(const Config* const cfg){
    Chip8* chip = calloc(1, sizeof(*chip));
    if(chip == nullptr) {
        LOG("Chip não alocado");
        return nullptr;
    }
    LOG("Chip alocado");
    LISTA(X)
    carregarSprites(chip);
    chip->program_counter = 0x200;
    srand((unsigned int)time(nullptr));
    return chip;
}

bool Chip8_delete(Chip8** chip){
    if(chip == nullptr || *chip == nullptr) {
        LOG("Chip não desalocado (nullptr)");
        return false;
    }
    free(*chip);
    *chip = nullptr;
    LOG("Chip desalocado");
    return true;
}

bool Chip8_load_rom(Chip8* chip, const char* const filepath){
    FILE* arq = fopen(filepath, "rb");
    if(arq == nullptr) {
        LOG("Não foi possível abrir a ROM no caminho '%s'", filepath);
        return false;
    }
    size_t lidos = fread(&chip->memoria[0x200], sizeof(chip->memoria[0x200]), 0x1000 - 0x200, arq);
    fclose(arq);
    if(lidos == 0) {
        LOG("Não foi possível carregar a ROM '%s'", filepath);
        return false;
    }
    LOG("ROM '%s' carregada (%zu bytes lidos)", filepath, lidos);
    return true;
}

void Chip8_update_keyboard(Chip8* chip, const bool* SDLKeys){
    const SDL_Scancode mapeamento_teclas[] = {
        SDL_SCANCODE_X, // 0
        SDL_SCANCODE_1, // 1
        SDL_SCANCODE_2, // 2
        SDL_SCANCODE_3, // 3
        SDL_SCANCODE_Q, // 4
        SDL_SCANCODE_W, // 5
        SDL_SCANCODE_E, // 6
        SDL_SCANCODE_A, // 7
        SDL_SCANCODE_S, // 8
        SDL_SCANCODE_D, // 9
        SDL_SCANCODE_Z, // A
        SDL_SCANCODE_C, // B
        SDL_SCANCODE_4, // C
        SDL_SCANCODE_R, // D
        SDL_SCANCODE_F, // E
        SDL_SCANCODE_V  // F
    };
    for(size_t i = 0; i < teclas_qtd; ++i){
        chip->teclas[i] = SDLKeys[mapeamento_teclas[i]];
    }
}

void getRender(Chip8* chip, uint32_t pixels[]){
    for(size_t i = 0; i < 64 * 32; ++i){
        pixels[i] = chip->display[i] ? 0xFFFFFFFF : 0x00FF;
    }
}

void Chip8_timers_decrement(Chip8* chip){
    if(chip->delay_timer > 0)
        chip->delay_timer--;
    if(chip->sound_timer > 0)
        chip->sound_timer--;
}

bool Chip8_get_draw(Chip8* chip){
    if(chip->draw){
        chip->draw = false;
        return true;
    }
    return false;
}

static inline void familia_0(Chip8* chip, uint8_t NN);
static inline void familia_1(Chip8* chip, uint16_t NNN);
static inline void familia_2(Chip8* chip, uint16_t NNN);
static inline void familia_3(Chip8* chip, uint8_t X, uint8_t NN);
static inline void familia_4(Chip8* chip, uint8_t X, uint8_t NN);
static inline void familia_5(Chip8* chip, uint8_t X, uint8_t Y);
static inline void familia_6(Chip8* chip, uint8_t X, uint8_t NN);
static inline void familia_7(Chip8* chip, uint8_t X, uint8_t NN);
static inline void familia_8(Chip8* chip, uint8_t X, uint8_t Y, uint8_t N);
static inline void familia_9(Chip8* chip, uint8_t X, uint8_t Y);
static inline void familia_A(Chip8* chip, uint16_t NNN);
static inline void familia_B(Chip8* chip, uint16_t NNN);
static inline void familia_C(Chip8* chip, uint8_t X, uint8_t NN);
static inline void familia_D(Chip8* chip, uint8_t X, uint8_t Y, uint8_t N);
static inline void familia_E(Chip8* chip, uint8_t X, uint8_t NN);
static inline void familia_F(Chip8* chip, uint8_t X, uint8_t NN);

void Chip8_cycle(Chip8* chip){
    // Fetch (busca)
    uint16_t opcode = (uint16_t)(chip->memoria[chip->program_counter] << 8) | chip->memoria[chip->program_counter + 1];
    chip->program_counter += 2;

    // Decode (decodificação)
    uint8_t nibble = (uint8_t)((opcode & 0XF000) >> 12);
    uint8_t X = (uint8_t)((opcode & 0x0F00) >> 8);
    uint8_t Y = (uint8_t)((opcode & 0x00F0) >> 4);
    uint8_t N = opcode & 0X000F;
    uint8_t NN = (uint8_t)(opcode & 0x00FF);
    uint16_t NNN = opcode & 0x0FFF;

    // Execute (executar)
    switch (nibble) {
        case 0x0: familia_0(chip, NN); break;
        case 0x1: familia_1(chip, NNN); break;
        case 0x2: familia_2(chip, NNN); break;
        case 0x3: familia_3(chip, X, NN); break;
        case 0x4: familia_4(chip, X, NN); break;
        case 0x5: familia_5(chip, X, Y); break;
        case 0x6: familia_6(chip, X, NN); break;
        case 0x7: familia_7(chip, X, NN); break;
        case 0x8: familia_8(chip, X, Y, N); break;
        case 0x9: familia_9(chip, X, Y); break;
        case 0xA: familia_A(chip, NNN); break;
        case 0xB: familia_B(chip, NNN); break;
        case 0xC: familia_C(chip, X, NN); break;
        case 0xD: familia_D(chip, X, Y, N); break;
        case 0xE: familia_E(chip, X, NN); break;
        case 0xF: familia_F(chip, X, NN); break;
        default: LOG("Instrução desconhecida");
    }

}

static inline void familia_0(Chip8* chip, uint8_t NN){
    if(NN == 0xE0){
        memset(chip->display, 0, sizeof(chip->display));
        chip->draw = true;
    }
    else if(NN == 0xEE){
        chip->program_counter = chip->stack[--chip->sp];
    }
}
static inline void familia_1(Chip8* chip, uint16_t NNN){
    chip->program_counter = NNN;
}
static inline void familia_2(Chip8* chip, uint16_t NNN){
    chip->stack[chip->sp++] = chip->program_counter;
    chip->program_counter = NNN; 
}
static inline void familia_3(Chip8* chip, uint8_t X, uint8_t NN){
    if(chip->registrador[X] == NN)
        chip->program_counter += 2;
}
static inline void familia_4(Chip8* chip, uint8_t X, uint8_t NN){
    if(chip->registrador[X] != NN)
        chip->program_counter += 2;
}
static inline void familia_5(Chip8* chip, uint8_t X, uint8_t Y){
    if(chip->registrador[X] == chip->registrador[Y])
        chip->program_counter += 2;
}
static inline void familia_6(Chip8* chip, uint8_t X, uint8_t NN){
    chip->registrador[X] = NN;
}
static inline void familia_7(Chip8* chip, uint8_t X, uint8_t NN){
    chip->registrador[X] += NN;
}
static inline void familia_8(Chip8* chip, uint8_t X, uint8_t Y, uint8_t N){
    switch (N) {
        // Operacoes logicas basicas
        case 0x0: chip->registrador[X] = chip->registrador[Y]; break;
        case 0x1: chip->registrador[X] |= chip->registrador[Y]; break;
        case 0x2: chip->registrador[X] &= chip->registrador[Y]; break;
        case 0x3: chip->registrador[X] ^= chip->registrador[Y]; break;

        // Operacoes aritmeticas
        case 0x4: {
            uint8_t estouro = chip->registrador[X] > (0x00FF - chip->registrador[Y]);
            chip->registrador[X] += chip->registrador[Y];
            chip->registrador[0xF] = estouro; break;
        }
        case 0x5:{
            uint8_t nao_negativo = chip->registrador[X] >= chip->registrador[Y];
            chip->registrador[X] -= chip->registrador[Y];
            chip->registrador[0xF] = nao_negativo; break;
        }
        case 0x7:{
            uint8_t nao_negativo = chip->registrador[Y] >= chip->registrador[X];
            chip->registrador[X] = chip->registrador[Y] - chip->registrador[X];
            chip->registrador[0xF] = nao_negativo; break;
        }

        // Shifts
        case 0x6:{
            uint8_t lsb = (chip->registrador[X] & 0x01);
            chip->registrador[X] >>= 1;
            chip->registrador[0xF] = lsb; break;
        }
        case 0xE:{
            uint8_t msb = chip->registrador[X] >> 7;
            chip->registrador[X] <<= 1;
            chip->registrador[0xF] = msb; break;
        }
        default: LOG("Operação desconhecida na família 8: %#X", N);
    }
}

static inline void familia_9(Chip8* chip, uint8_t X, uint8_t Y){
    if(chip->registrador[X] != chip->registrador[Y])
        chip->program_counter += 2;
}
static inline void familia_A(Chip8* chip, uint16_t NNN){
    chip->registrador_de_indice = NNN;
}
static inline void familia_B(Chip8* chip, uint16_t NNN){
    chip->program_counter = NNN + chip->registrador[0];
}
static inline void familia_C(Chip8* chip, uint8_t X, uint8_t NN){
    chip->registrador[X] = (uint8_t)(rand() % 256) & NN;
}
static inline void familia_D(Chip8* chip, uint8_t X, uint8_t Y, uint8_t N){
    uint8_t x = chip->registrador[X] % 64; 
    uint8_t y = chip->registrador[Y] % 32;

    chip->registrador[0xF] = 0;

    for(int linha = 0; linha < N; linha++){
        uint8_t pixel_do_sprite = chip->memoria[chip->registrador_de_indice + linha];

        if(y + linha >= 32) break;
        
        for(int coluna = 0; coluna < 8; coluna++){
            if(x + coluna >= 64) break;

            if((pixel_do_sprite & (0x80 >> coluna)) != 0){
                int indice_display = (y + linha) * 64 + (x + coluna);
                
                if(chip->display[indice_display] == 1)
                    chip->registrador[0xF] = 1;

                chip->display[indice_display] ^= 1;
            }
        }
    }
    
    chip->draw = true;
}
static inline void familia_E(Chip8* chip, uint8_t X, uint8_t NN){
    uint8_t tecla_alvo = chip->registrador[X] & 0x0F;
    switch (NN) {
        case 0x9E:
            if (chip->teclas[tecla_alvo]) 
                chip->program_counter += 2;
            break;
        case 0xA1:
            if (!chip->teclas[tecla_alvo]) 
                chip->program_counter += 2;
            break;
    }
}
static inline void familia_F(Chip8* chip, uint8_t X, uint8_t NN){
    switch (NN) {
        case 0x07: chip->registrador[X] = chip->delay_timer; break;
        case 0x15: chip->delay_timer = chip->registrador[X]; break;
        case 0x18: chip->sound_timer = chip->registrador[X]; break;

        case 0x1E:{
            chip->registrador_de_indice += chip->registrador[X];
            if (chip->amiga) 
                chip->registrador[0xF] = (chip->registrador_de_indice > 0xFFF) ? 1 : 0;
            break;
        }
        case 0x29:
            chip->registrador_de_indice = (uint16_t)(0x50 + 5 * (chip->registrador[X] & 0x0F));
        break;
        case 0x33:
            chip->memoria[chip->registrador_de_indice] = chip->registrador[X] / 100;
            chip->memoria[chip->registrador_de_indice + 1] = (chip->registrador[X] % 100) / 10;
            chip->memoria[chip->registrador_de_indice + 2] = chip->registrador[X] % 10;
            break;
        case 0x55:{  
            uint16_t i;
            for(i = 0; i <= X; i++)
                chip->memoria[chip->registrador_de_indice + i] = chip->registrador[i];
            if(chip->retro)
                chip->registrador_de_indice = (chip->registrador_de_indice + i) & 0xFFF;
            break;
        }
        case 0x65:{
            uint16_t i;
            for(i = 0; i <= X; i++)
                chip->registrador[i] = chip->memoria[chip->registrador_de_indice + i];
            if(chip->retro)
                chip->registrador_de_indice = (chip->registrador_de_indice + i) & 0xFFF;
            break;
        }
        case 0x0A:{
            bool tecla_pressionada = false;
            for(uint8_t i = 0; i < teclas_qtd; ++i){
                if(chip->teclas[i]){
                    chip->registrador[X] = i;
                    tecla_pressionada = true;
                    break;
                }
            }
            if(!tecla_pressionada) chip->program_counter -= 2;
            break;
        }
        default: LOG("Instrução desconhecida da familia F: %#X", NN);
    }
}
