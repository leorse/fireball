#include <SDL2/SDL.h>

#include "affichage.h"
#include "particule.h"
#include "contexte.h"

void putPixel(int x, int y, int couleur, SDL_Surface *surface)
{
    if (x < 1 || y < 1 || x >= L - 1 || y >= H - 1)
    {
        return;
    }
    uint8_t *offscreen = (uint8_t *)surface->pixels;
    offscreen[y * L + x] = couleur;
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

void afficherBoard(CONTEXTE *contexte, SDL_Surface *surface)
{
    char texte[50];
    SDL_Rect dstRect = {100, 0 ,50, 200};

    sprintf(texte, "Nombre Particule:%d", contexte->nombreMeteor);
    afficherTexte(texte, contexte, surface, dstRect);
}

void afficherTexte(char* texte, CONTEXTE* contexte, SDL_Surface *surface, SDL_Rect rectDest)
{
    SDL_Color color = {0, 0, 0};
    SDL_Surface *text_surface;

    int w,h;
    TTF_SizeUTF8(contexte->font,texte,&w,&h);
    rectDest.w = w;
    rectDest.h = h;
    if (!(text_surface = TTF_RenderUTF8_Solid(contexte->font, texte, color)))
    {
        //handle error here, perhaps print TTF_GetError at least
    }
    else
    {
        SDL_FillRect( surface, &rectDest, 0);//SDL_MapRGB( surface->format, 0, 0, 0 ) );
        SDL_BlitSurface(text_surface, NULL, surface, &rectDest);
        //perhaps we can reuse it, but I assume not for simplicity.
        SDL_FreeSurface(text_surface);
    }
}