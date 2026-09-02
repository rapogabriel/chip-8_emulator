#pragma once

typedef struct _chip8 Chip8;

Chip8* Chip8_new();
bool Chip8_load_rom(Chip8*, const char* filepath);
void Chip8_delete(Chip8**);