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
        //handle a left-click
        printf("BOUTON!!!!!! %d,%d\n", bE->x, bE->y);
        addParticule(contexte, bE->x, bE->y);
        // SDL_Cursor cur = SDL_GetCursor
    }
}

void keyPress(SDL_KeyboardEvent *kE, CONTEXTE *contexte)
{
    SDL_Keysym ks = kE->keysym;
    SDL_Log("Physical %s key acting as %s key",
            SDL_GetScancodeName(kE->keysym.scancode),
            SDL_GetKeyName(kE->keysym.sym));
    if (kE->keysym.scancode == SDL_SCANCODE_P)
    {
        printf("BOUTON P\n");
        switchPalette(contexte);
    }
}

void afficherInfosTimer(clock_t t1, clock_t t2, long nbFrame)
{
    long clk_tck = CLOCKS_PER_SEC;

    double cumul = 0;
    double moyenne = 0;
    double tickMin, tickMax = 0;

    double tick = (double)(t2 - t1) / (double)clk_tck;
    if (nbFrame == 100)
    {
        cumul = 0;
        nbFrame = 1;
        tickMax = tickMin = 0;
    }
    cumul += tick;
    moyenne = cumul / nbFrame;
    if (tick > tickMax || tickMax == 0)
    {
        tickMax = tick;
    }
    if (tick < tickMin || tickMin == 0)
    {
        tickMin = tick;
    }
    //(void)printf("Temps consomme (s) : %lf , moyenne : %lf, min: %lf, Max: %lf\n", tick, moyenne, tickMin, tickMax);
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
                //do whatever you want to do after a mouse button was pressed,
                // e.g.:
                mousePress(&event.button, &contexte);
            }
            if (event.type == SDL_KEYUP)
            {
                keyPress(&event.button, &contexte);
            }
        }
        moveParticule(contexte.premier, &contexte);
        drawParticule(surface, contexte.Sprites, contexte.premier);
        blur(1, 1, L - 1, H - 1, surface);
        if(contexte.drawPalette)
        {
            afficherPalette(surface);
        }
        SDL_Texture *texture = SDL_CreateTextureFromSurface(sdlRenderer, surface);
        SDL_RenderCopy(sdlRenderer, texture, NULL, NULL);
        SDL_RenderPresent(sdlRenderer);
        //pour effacer
        //SDL_FillRect( surface, NULL, SDL_MapRGB( surface->format, 0, 0, 0 ) );
        SDL_DestroyTexture(texture);

        /* Recuperation du temps final en "clock ticks" */
        t2 = clock();

        afficherInfosTimer(t1, t2, nbFrame);
    }

    SDL_DestroyRenderer(sdlRenderer);
    detruireContexte(&contexte);
    SDL_Quit();

    return 0;
}