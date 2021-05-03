#include "initSDL.h"
#include "contexte.h"

bool initSDL(CONTEXTE *contexte, SDL_Renderer **sdlRenderer)
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

    contexte->surface = SDL_CreateRGBSurface(SDL_SWSURFACE, L, H, 8, 0, 0, 0, 0);
    if(contexte->surface == NULL)
    {
        printf("ça ne veut pas créer surface");
        return false;
    }
    contexte->bump = SDL_CreateRGBSurface(SDL_SWSURFACE, L, H, 8, 0, 0, 0, 0);
    if(contexte->bump == NULL)
    {
        printf("ça ne veut pas créer bump");
        return false;
    }
    contexte->phongmap = SDL_CreateRGBSurface(SDL_SWSURFACE, L, H, 8, 0, 0, 0, 0);
    if(contexte->phongmap == NULL)
    {
        printf("ça ne veut pas créer phongmap");
        return false;
    }
    contexte->cache = SDL_CreateRGBSurface(SDL_SWSURFACE, L, H, 8, 0, 0, 0, 0);
    if(contexte->cache == NULL)
    {
        printf("ça ne veut pas créer cache");
        return false;
    }
    SDL_SetPaletteColors((contexte->surface)->format->palette, contexte->listeCouleur, 0, 256);
    SDL_SetPaletteColors((contexte->bump)->format->palette, contexte->listeCouleur, 0, 256);
    SDL_SetPaletteColors((contexte->phongmap)->format->palette, contexte->listeCouleur, 0, 256);
    SDL_SetPaletteColors((contexte->cache)->format->palette, contexte->listeCouleur, 0, 256);
}