#include <SDL2/SDL.h>

#include "affichage.h"
#include "particule.h"
#include "contexte.h"
#include "logo.h"

void putPixel(int x, int y, int couleur, SDL_Surface *surface)
{
    if (x < 1 || y < 1 || x >= L - 1 || y >= H - 1)
    {
        return;
    }
    if (couleur < 0)
    {
        couleur = 0;
    }
    if (couleur > 255)
    {
        couleur = 255;
    }
    uint8_t *offscreen = (uint8_t *)surface->pixels;
    int offset = y * L + x;
    if (offscreen[y * L + x] < couleur)
    {
        offscreen[y * L + x] = couleur;
    }
}

void blur(int x1, int y1, int x2, int y2, SDL_Surface *surface)
{
    int x, y;
    int resultat;

    uint8_t *VScreen = (uint8_t *)surface->pixels;
    for (x = x1; x < x2; x++)
    {
        for (y = y1; y < y2; y++)
        {
            resultat =
                VScreen[(y - 1) * L + x] +
                VScreen[(y + 1) * L + x] +
                VScreen[y * L + (x + 1)] +
                VScreen[y * L + (x - 1)];
            resultat = resultat / 4;
            VScreen[y * L + x] = resultat;
        }
    }
}

void drawParticule(SDL_Surface *VScreen, bool *Sprites[MAX_TAILLE], PARTICULE *particule)
{
    PARTICULE *ptrParticule;
    for (ptrParticule = particule; ptrParticule != NULL; ptrParticule = ptrParticule->suivant)
    {
        ptrParticule->Drawer(ptrParticule, VScreen, Sprites);
    }
}

void afficherLogoTopRight(SDL_Surface *VScreen)
{
    afficherLogo(L - LOGO_W, 0, VScreen);
}

void afficherLogoCenter(SDL_Surface *VScreen)
{
    afficherLogo((L / 2) - (LOGO_W / 2), (H / 2) - (LOGO_H / 2), VScreen);
}

void afficherLogo(int x, int y, SDL_Surface *VScreen)
{
    for (int incX = 0; incX < LOGO_W; incX++)
    {
        for (int incY = 0; incY < LOGO_H; incY++)
        {
            if (c_logo_map[incY * LOGO_W + incX] != 0x00)
            {
                putPixel(incX + x, incY + y, 255, VScreen);
            }
        }
    }
}

void afficherPalette(SDL_Surface *VScreen)
{
    for (int incX = 0; incX < 40; incX++)
    {
        for (int incY = 0; incY < H; incY++)
        {
            putPixel(incX, incY, incY * 256 / H, VScreen);
        }
    }
}

char *calculerInfosTimer(char *texte, clock_t t1, clock_t t2, long nbFrame)
{
    long clk_tck = CLOCKS_PER_SEC;

    static double cumul = 0;
    double moyenne = 0;
    double tickMin, tickMax = 0;

    double tick = (double)(t2 - t1) / (double)clk_tck;
    double fps = (t2 - t1) == 0 ? 1000 : 1000 / ((double)(t2 - t1) * 1000 / (double)clk_tck);
    if (nbFrame == 100)
    {
        cumul = 0;
        nbFrame = 1;
        tickMax = tickMin = 0;
    }
    cumul += fps;
    moyenne = cumul / nbFrame;
    if (tick > tickMax || tickMax == 0)
    {
        tickMax = tick;
    }
    if (tick < tickMin || tickMin == 0)
    {
        tickMin = tick;
    }
    sprintf(texte, "FPS : %ld, moyenne: %lf", (long)fps, moyenne);
}

void afficherBoard(CONTEXTE *contexte, SDL_Surface *surface, clock_t t1, clock_t t2, long nbFrame)
{
    char texte[100];
    SDL_Rect dstRect = {100, 0, 50, 200};

    sprintf(texte, "Nombre Particule:%d", contexte->nombreMeteor);

    afficherTexte(texte, contexte, surface, dstRect);
    calculerInfosTimer(texte, t1, t2, nbFrame);
    dstRect.y = 25;
    afficherTexte(texte, contexte, surface, dstRect);
}

void afficherTexte(char *texte, CONTEXTE *contexte, SDL_Surface *surface, SDL_Rect rectDest)
{
    SDL_Color color = {255, 255, 255};
    SDL_Surface *text_surface;

    int w, h;
    TTF_SizeUTF8(contexte->font, texte, &w, &h);
    rectDest.w = w;
    rectDest.h = h;
    if (!(text_surface = TTF_RenderUTF8_Solid(contexte->font, texte, color)))
    {
        //handle error here, perhaps print TTF_GetError at least
    }
    else
    {
        SDL_FillRect(surface, &rectDest, 0);
        SDL_BlitSurface(text_surface, NULL, surface, &rectDest);
        SDL_FreeSurface(text_surface);
    }
}

void initialiserLumiere(CONTEXTE *contexte)
{
    int taille = TAILLE_LUMIERE;
    //printf("L:%d, H:%d\n", L,H);
    for (int x = 0; x < L; x++)
    {
        float deltaX = (L / 2) - x;
        float longX = deltaX * deltaX;
        for (int y = 0; y < H; y++)
        {
            float deltaY = (H / 2) - y;
            float longY = deltaY * deltaY;
            float longueur = sqrt(longX + longY);
            int hauteur = 0;
            if (longueur <= taille)
            {
                hauteur = 256 - (longueur * 256 / taille);
            }
            putPixel(x, y, hauteur, contexte->phongmap);
        }
    }
}

void drawBumpMapping(CONTEXTE *contexte, int x, int y)
{
    int xdist, ydist;
    int xdelta, ydelta;
    int xtemp, ytemp, temp;
    int incY, incX, offset = L;
    int lx, ly;
    unsigned int u, v;
    uint8_t *source = (uint8_t *)contexte->bump->pixels;
    uint8_t *dest = (uint8_t *)contexte->surface->pixels;
    uint8_t *phonglightmap = (uint8_t *)contexte->phongmap->pixels;

    ly = -(y - TAILLE_LUMIERE);
    for (incY = 1; incY < H - 2; incY++)
    {
        lx = -(x - TAILLE_LUMIERE);
        for (incX = 0; incX < L; incX++, offset++)
        {
            xdelta = ((source[incY * L + (incX - 1)] - source[incY * L + (incX)]));
            ydelta = ((source[incY * L + incX] - source[(incY + 1) * L + incX]));
            lx++;

            xtemp = xdelta + lx;
            ytemp = ydelta + ly;
            if (xtemp < 0 || xtemp >= L || ytemp < 0 || ytemp >= H)
            {
                *dest++ = 0;
            }
            else
            {
                u = ytemp;
                v = xtemp;
                *dest++ = phonglightmap[v * L + u];
            }
        }
        ly++;
    }
}