#include <stdio.h>
#include <stdbool.h>
#include <time.h>

/* GL/glew.h must be included before SDL2/SDL.h */
#include <GL/glew.h>
#include <SDL2/SDL.h>

#include "contexte.h"
#include "particule.h"
#include "initSDL.h"
#include "affichage.h"
#include "shader.h"

static void moveParticule(CONTEXTE *contexte)
{
    for (PARTICULE *p = contexte->premier; p != NULL; p = p->suivant)
    {
        bool retour = p->Grower(p);
        if (retour)
        {
            if (p->explosive)
            {
                addParticule(contexte, 10,
                             (int)(p->x + p->refX),
                             (int)(p->y + p->refY));
                p->explosive = false;
            }
            else
            {
                p = removeParticule(contexte, p);
            }
        }
    }
}

static void mousePress(SDL_MouseButtonEvent *bE, CONTEXTE *contexte)
{
    if (bE->button == SDL_BUTTON_LEFT)
        addParticule(contexte, 50, bE->x, bE->y);
}

static void keyPress(SDL_KeyboardEvent *kE, CONTEXTE *contexte)
{
    switch (kE->keysym.scancode)
    {
        case SDL_SCANCODE_P: switchPalette(contexte); break;
        case SDL_SCANCODE_B: switchBoard(contexte);   break;
        case SDL_SCANCODE_F: switchBlur(contexte);    break;
        case SDL_SCANCODE_L: switchLogo(contexte);    break;
        case SDL_SCANCODE_M: switchMode(contexte);    break;
        default: break;
    }
}

int main(int argc, char *argv[])
{
    (void)argc; (void)argv;

    SDL_Event event;
    bool      exitProgram = false;
    CONTEXTE  contexte;

    long    nbFrame = 0L;
    clock_t t1, t2;
    int     mouseX = 0, mouseY = 0;

    initContexte(&contexte, 1000, 1000);

    if (!initSDL(&contexte))
    {
        fprintf(stderr, "initSDL a échoué\n");
        return 1;
    }
    if (!initGL(&contexte))
    {
        fprintf(stderr, "initGL a échoué\n");
        return 1;
    }

    while (!exitProgram)
    {
        nbFrame++;
        t1 = clock();

        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_QUIT)
            {
                exitProgram = true;
                break;
            }
            if (event.type == SDL_MOUSEBUTTONDOWN)
                mousePress(&event.button, &contexte);
            if (event.type == SDL_KEYUP)
                keyPress(&event.key, &contexte);
            if (event.type == SDL_MOUSEMOTION)
            {
                mouseX = event.motion.x;
                mouseY = event.motion.y;
            }
            if (event.type == SDL_WINDOWEVENT &&
                event.window.event == SDL_WINDOWEVENT_RESIZED)
            {
                int nouvW = event.window.data1;
                int nouvH = event.window.data2;
                redimensionnerContexte(&contexte, nouvW, nouvH);
            }
        }

        /* Particle update and CPU drawing onto bump surface */
        moveParticule(&contexte);
        drawParticule(contexte.bump, contexte.Sprites, contexte.premier);

        if (contexte.drawBlur)
            blur(1, 1, contexte.largeur - 1, contexte.hauteur - 1, contexte.bump);

        if (contexte.drawLogo)
            afficherLogoTopRight(contexte.bump);

        t2 = clock();

        /* GPU rendering */
        glClear(GL_COLOR_BUFFER_BIT);
        rendreGPU(&contexte, mouseX, mouseY, t1, t2, nbFrame);

        SDL_GL_SwapWindow(contexte.window);

        /* Clear bump next frame if blur is off (avoids particle trails accumulating) */
        if (!contexte.drawBlur)
            SDL_FillRect(contexte.bump, NULL, 0);
    }

    detruireContexte(&contexte);

    return 0;
}
