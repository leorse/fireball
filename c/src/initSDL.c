#include <stdio.h>
#include <stdlib.h>

#include "initSDL.h"
#include "contexte.h"

bool initSDL(CONTEXTE *contexte)
{
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        fprintf(stderr, "SDL_Init: %s\n", SDL_GetError());
        return false;
    }
    if (TTF_Init() < 0)
    {
        fprintf(stderr, "TTF_Init: %s\n", TTF_GetError());
        return false;
    }

    contexte->font = TTF_OpenFont("FreeSans.otf", 16);
    if (!contexte->font)
    {
        fprintf(stderr, "TTF_OpenFont: %s\n", TTF_GetError());
    }

    /* Request OpenGL 3.3 Core Profile */
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

    contexte->window = SDL_CreateWindow(
        "Fireball",
        SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
        contexte->largeur, contexte->hauteur,
        SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);

    if (!contexte->window)
    {
        fprintf(stderr, "SDL_CreateWindow: %s\n", SDL_GetError());
        return false;
    }

    contexte->glContext = SDL_GL_CreateContext(contexte->window);
    if (!contexte->glContext)
    {
        fprintf(stderr, "SDL_GL_CreateContext: %s\n", SDL_GetError());
        return false;
    }

    /* VSync */
    SDL_GL_SetSwapInterval(1);

    /* CPU surfaces — 8-bit greyscale (no SDL palette needed for GL rendering) */
    contexte->bump = SDL_CreateRGBSurface(SDL_SWSURFACE,
                                          contexte->largeur, contexte->hauteur,
                                          8, 0, 0, 0, 0);
    if (!contexte->bump)
    {
        fprintf(stderr, "SDL_CreateRGBSurface (bump): %s\n", SDL_GetError());
        return false;
    }

    contexte->cache = SDL_CreateRGBSurface(SDL_SWSURFACE,
                                           contexte->largeur, contexte->hauteur,
                                           8, 0, 0, 0, 0);
    if (!contexte->cache)
    {
        fprintf(stderr, "SDL_CreateRGBSurface (cache): %s\n", SDL_GetError());
        return false;
    }

    /* RGBA32 HUD overlay for text and palette bar */
    contexte->hud = SDL_CreateRGBSurfaceWithFormat(0,
                                                   contexte->largeur, contexte->hauteur,
                                                   32, SDL_PIXELFORMAT_RGBA32);
    if (!contexte->hud)
    {
        fprintf(stderr, "SDL_CreateRGBSurfaceWithFormat (hud): %s\n", SDL_GetError());
        return false;
    }

    return true;
}
