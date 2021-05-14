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


void moveParticule(CONTEXTE *contexte)
{
    int i;
    float temps;
    PARTICULE *ptrParticule;
    PARTICULE *particule = contexte->premier;

    i = 0;
    for (ptrParticule = particule; ptrParticule != NULL; ptrParticule = ptrParticule->suivant)
    {
        bool retour = ptrParticule->Grower(ptrParticule);
        if (retour)
        {
            if (ptrParticule->explosive)
            {
                addParticule(contexte, 10, ptrParticule->x+ptrParticule->refX, ptrParticule->y+ptrParticule->refY);
                //une particule ne peut exploquer qu'une seule fois
                ptrParticule->explosive = false;
            }
            else
            {
                ptrParticule = removeParticule(contexte, ptrParticule);
            }
        }
    }
}

void mousePress(SDL_MouseButtonEvent *bE, CONTEXTE *contexte)
{
    if (bE->button == SDL_BUTTON_LEFT)
    {
        addParticule(contexte, 50, bE->x, bE->y);
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
    if (kE->keysym.scancode == SDL_SCANCODE_L)
    {
        switchLogo(contexte);
    }
    if (kE->keysym.scancode == SDL_SCANCODE_M)
    {
        switchMode(contexte);
    }
}

int main(int argv, char *argc[])
{

    SDL_Event event;

    SDL_Renderer *sdlRenderer;
    bool exitProgram = false;
    CONTEXTE contexte;

    long nbFrame = 0L;
    clock_t t1, t2;
    int mouseX, mouseY;

    initContexte(&contexte);
    initSDL(&contexte, &sdlRenderer);

    //initialiserLumiere(&contexte);
    //exitProgram=true;
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
            if (event.type == SDL_MOUSEMOTION)
            {
                mouseX = event.motion.x;
                mouseY = event.motion.y;
            }
        }

        //l'affichage des particules se fait sur bump
        moveParticule(&contexte);
        drawParticule(contexte.bump, contexte.Sprites, contexte.premier);

        if (contexte.drawBlur)
        {
            blur(1, 1, L - 1, H - 1, contexte.bump);
        }
        if (contexte.mode == FIREBALL)
        {
            SDL_BlitSurface(contexte.bump, NULL, contexte.surface, NULL);
        }
        if (contexte.drawPalette)
        {
            afficherPalette(contexte.surface);
        }
        if (contexte.drawLogo)
        {
            afficherLogoTopRight(contexte.surface);
        }
        /* Recuperation du temps final en "clock ticks" */

        if (contexte.drawBoard)
        {
            t2 = clock();
            afficherBoard(&contexte, contexte.surface, t1, t2, nbFrame);
        }

        if (contexte.mode == LIGHT)
        {
            drawBumpMapping(&contexte, mouseX, mouseY);
        }

        SDL_Texture *texture = SDL_CreateTextureFromSurface(sdlRenderer, contexte.surface);
        SDL_RenderCopy(sdlRenderer, texture, NULL, NULL);

        SDL_RenderPresent(sdlRenderer);
        //pour effacer
        if (!contexte.drawBlur)
        {
            SDL_FillRect(contexte.surface, NULL, 0);
        }
        SDL_DestroyTexture(texture);
    }

    SDL_DestroyRenderer(sdlRenderer);
    detruireContexte(&contexte);
    SDL_Quit();

    return 0;
}