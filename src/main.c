
#include <SDL3/SDL.h>
#include "core.h"

int main(void){
    App app = {0};

    if (appInit(&app, "Chip-8 Emulator", 800, 600)){
        appRun(&app);
        appDestroy(&app);
    }
}