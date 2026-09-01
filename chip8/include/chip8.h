#pragma once

#include <stdint.h>
#include "config.h"

typedef struct _Chip8 Chip8;

Chip8* Chip8_new(const Config* const cfg);
bool Chip8_load_rom(Chip8* chip, const char* const filepath);
void Chip8_cycle(Chip8* chip);
bool Chip8_delete(Chip8** chip);
void Chip8_update_keyboard(Chip8* chip, const bool* SDLKeys);
void getRender(Chip8* chip, uint32_t pixels[]);
void Chip8_timers_decrement(Chip8* chip);
bool Chip8_get_draw(Chip8* chip);