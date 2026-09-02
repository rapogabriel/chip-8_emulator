#include <stdlib.h>
#include "chip-8.h"

struct _chip8 {
    const char* a;
};

Chip8* Chip8_new(){
    Chip8* ret = malloc(sizeof(*ret));
    return ret;
}
bool Chip8_load_rom(Chip8*, const char* filepath);
void Chip8_delete(Chip8** chip){
    if(chip == nullptr || *chip == nullptr) return;
    free(*chip);
    *chip = nullptr;
}