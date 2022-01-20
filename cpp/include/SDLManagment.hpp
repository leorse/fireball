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
    SDL_Surface *phongmap;
    SDL_Surface *bump;
    SDL_Surface *cache;

    void render(void);
    SDLManagment();
    ~SDLManagment();
};
