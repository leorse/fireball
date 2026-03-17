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
                addParticule(contexte, 10, ptrParticule->x + ptrParticule->refX, ptrParticule->y + ptrParticule->refY);
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

    initContexte(&contexte, 1000, 1000);
    initSDL(&contexte, &sdlRenderer);

    SDL_Texture *texture = SDL_CreateTexture(sdlRenderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STREAMING, contexte.largeur, contexte.hauteur);

    //initialiserLumiere(&contexte);
    //exitProgram=true;
    while (!exitProgram)
    {
        nbFrame++;
        Uint32 frameStart = SDL_GetTicks();
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
            if (event.type == SDL_WINDOWEVENT && event.window.event == SDL_WINDOWEVENT_RESIZED)
            {
                int nouvW = event.window.data1;
                int nouvH = event.window.data2;
                redimensionnerContexte(&contexte, nouvW, nouvH);
                SDL_DestroyTexture(texture);
                texture = SDL_CreateTexture(sdlRenderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STREAMING, nouvW, nouvH);
            }
        }

        //l'affichage des particules se fait sur bump
        moveParticule(&contexte);
        drawParticule(contexte.bump, contexte.Sprites, contexte.premier);

        if (contexte.drawBlur)
        {
            blur(1, 1, contexte.largeur - 1, contexte.hauteur - 1, contexte.bump);
        }
        if (contexte.drawLogo)
        {
            afficherLogoTopRight(contexte.bump);
        }
        if (contexte.mode == FIREBALL)
        {
            SDL_BlitSurface(contexte.bump, NULL, contexte.surface, NULL);
        }

        if (contexte.drawPalette)
        {
            afficherPalette(contexte.surface);
        }
        /* Recuperation du temps final en "clock ticks" */

        if (contexte.drawBoard)
        {
            t2 = clock();
            afficherBoard(&contexte, contexte.surface, t1, t2, nbFrame);
        }

        if (contexte.mode == LIGHT)
        {
            doModeLight(&contexte, mouseX, mouseY);
        }
        if (contexte.mode == SHADOW)
        {
            doModeShadow(&contexte, mouseX, mouseY);
        }
        if (contexte.mode == GLASS)
        {
            doModeGlass(&contexte);
        }
        SDL_Surface *tmp = SDL_ConvertSurfaceFormat(contexte.surface, SDL_PIXELFORMAT_ARGB8888, 0);
        SDL_UpdateTexture(texture, NULL, tmp->pixels, tmp->pitch);
        SDL_FreeSurface(tmp);
        SDL_RenderCopy(sdlRenderer, texture, NULL, NULL);

        SDL_RenderPresent(sdlRenderer);
        //pour effacer
        if (!contexte.drawBlur)
        {
            SDL_FillRect(contexte.surface, NULL, 0);
        }

        Uint32 frameTime = SDL_GetTicks() - frameStart;
        if (frameTime < 16)
        {
            SDL_Delay(16 - frameTime);
        }
    }

    SDL_DestroyTexture(texture);
    SDL_DestroyRenderer(sdlRenderer);
    detruireContexte(&contexte);
    SDL_Quit();

    return 0;
}