

#include "render.h"
#include "SDL3/SDL_rect.h"
#include "SDL3/SDL_render.h"

typedef struct btn {
    SDL_FRect* area;
    SDL_Texture* imagem;
    bool hover;
    bool click;
} Btn;

// ({x, y, w, h}, nome_arquivo)
#define BOTOES(X) \
X({800.0f - 16.0f - 10.0f, (36.0f - 16.0f) / 2.0f, 16.0f, 16.0f}, "close_icon.png")

