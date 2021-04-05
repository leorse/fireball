#include "initSDL.h"
#include "contexte.h"

bool initSDL(CONTEXTE *contexte, SDL_Surface **surface, SDL_Renderer **sdlRenderer)
{
    SDL_Window *sdlWindow;

    if (SDL_Init(SDL_INIT_EVERYTHING) < 0)
    {
        return false;
    }
    if (TTF_Init() < 0)
    {
        printf("Erreur d'initialisation de TTF_Init : %s\n", TTF_GetError());
        exit(EXIT_FAILURE);
    }

    // load font.ttf at size 16 into font
    
    contexte->font = TTF_OpenFont("FreeSans.otf", 16);
    if (!contexte->font)
    {
        printf("TTF_OpenFont: %s\n", TTF_GetError());
        // handle error
    }

    if ((sdlWindow = SDL_CreateWindow("ma fenêtre", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, L, H, SDL_WINDOW_OPENGL)) == NULL)
    {
        printf("non ça veut pas créer!");
        return false;
    }

    *sdlRenderer = SDL_CreateRenderer(sdlWindow, -1, 0);

    *surface = SDL_CreateRGBSurface(SDL_SWSURFACE, L, H, 8, 0, 0, 0, 0);
    SDL_SetPaletteColors((*surface)->format->palette, contexte->listeCouleur, 0, 256);
}