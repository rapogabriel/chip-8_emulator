

#include "btn_actions.h"
#include "SDL3/SDL_video.h"
#include "log.h"

void act_close(App* app){
    app->aberta = false;
    sucesso;
}

void act_maxres(App* app){
    if(app->maximizado){
        SDL_RestoreWindow(app->window);
        app->maxres.corte.x = 0.0f;
    } else{
        SDL_MaximizeWindow(app->window);
        app->maxres.corte.x = 16.0f;
    }
    app->maximizado = !app->maximizado;
    sucesso;
}