#pragma once

#include "core.h"

// (nomeVar, x, y, w, h, nome_arquivo)
#define BOTOES(X) \
X(close, 800.0f - 16.0f - 10.0f, (36.0f - 16.0f) / 2.0f, 16.0f, 16.0f, "close_icon.png")

bool initBtns(App* app);
bool drawRender(App* app);
void destroyBtns(App* app);