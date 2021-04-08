#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

class SDLManagment
{

private:
    SDL_Window *sdlWindow;
    SDL_Color palette[256];
    void initPalette();

public:
    static SDL_Renderer *sdlRenderer;
    static SDL_Surface *surface;

    SDLManagment();
    ~SDLManagment();
};
