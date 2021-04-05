#include <stdio.h>
#include <stdbool.h>
#include <math.h>
#include <time.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_mouse.h>

#include "contexte.h"
#include "particule.h"
#include "initSDL.h"
#include "affichage.h"

void moveParticule(PARTICULE *particule, CONTEXTE *contexte)
{
    int i;
    float temps;
    PARTICULE *ptrParticule;

    i = 0;
    for (ptrParticule = particule; ptrParticule != NULL; ptrParticule = ptrParticule->suivant)
    {
        bool retour = ptrParticule->Grower(ptrParticule);
        if (retour)
        {
            ptrParticule = removeParticule(contexte, ptrParticule);
        }
    }
}

void mousePress(SDL_MouseButtonEvent *bE, CONTEXTE *contexte)
{
    if (bE->button == SDL_BUTTON_LEFT)
    {
        addParticule(contexte, bE->x, bE->y);
    }
}

void keyPress(SDL_KeyboardEvent *kE, CONTEXTE *contexte)
{
    SDL_Keysym ks = kE->keysym;
    if (kE->keysym.scancode == SDL_SCANCODE_P)
    {
        switchPalette(contexte);
    }
    if (kE->keysym.scancode == SDL_SCANCODE_B)
    {
        switchBoard(contexte);
    }
    if (kE->keysym.scancode == SDL_SCANCODE_F)
    {
        switchBlur(contexte);
    }
}



int main(int argv, char *argc[])
{

    SDL_Event event;
    SDL_Surface *surface;
    SDL_Renderer *sdlRenderer;
    bool exitProgram = false;
    CONTEXTE contexte;

    long nbFrame = 0L;
    clock_t t1, t2;

    initContexte(&contexte);
    initSDL(&contexte, &surface, &sdlRenderer);

    while (!exitProgram)
    {
        nbFrame++;
        /* Recuperation du temps initial en "clock ticks" */
        t1 = clock();

        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_QUIT)
            {
                exitProgram = true;
                break;
            }

            if (event.type == SDL_MOUSEBUTTONDOWN)
            {
                mousePress(&event.button, &contexte);
            }
            if (event.type == SDL_KEYUP)
            {
                keyPress(&event.key, &contexte);
            }
        }
        moveParticule(contexte.premier, &contexte);
        drawParticule(surface, contexte.Sprites, contexte.premier);
        if (contexte.drawBlur)
        {
            blur(1, 1, L - 1, H - 1, surface);
        }
        if (contexte.drawPalette)
        {
            afficherPalette(surface);
        }
        /* Recuperation du temps final en "clock ticks" */
        
        if (contexte.drawBoard)
        {
            t2 = clock();
            afficherBoard(&contexte, surface, t1, t2, nbFrame);
        }

        SDL_Texture *texture = SDL_CreateTextureFromSurface(sdlRenderer, surface);
        SDL_RenderCopy(sdlRenderer, texture, NULL, NULL);

        SDL_RenderPresent(sdlRenderer);
        //pour effacer
        if (!contexte.drawBlur)
        {
            SDL_FillRect( surface, NULL, 0 );
        }
        SDL_DestroyTexture(texture);
        
    }

    SDL_DestroyRenderer(sdlRenderer);
    detruireContexte(&contexte);
    SDL_Quit();

    return 0;
}