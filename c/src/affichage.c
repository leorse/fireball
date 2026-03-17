#include <SDL2/SDL.h>

#include "affichage.h"
#include "particule.h"
#include "contexte.h"
#include "logo.h"

void putPixel(int x, int y, int couleur, bool force, SDL_Surface *surface)
{
    if (x < 1 || y < 1 || x >= surface->w - 1 || y >= surface->h - 1)
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
    int offset = y * surface->pitch + x;
    if (force || offscreen[offset] < couleur)
    {
        offscreen[offset] = couleur;
    }
}

void blur(int x1, int y1, int x2, int y2, SDL_Surface *surface)
{
    int x, y;
    int resultat;
    int pitch = surface->pitch;
    int offsetY = y1 * pitch;

    uint8_t *VScreen = (uint8_t *)surface->pixels;
    for (y = y1; y < y2; y++, offsetY += pitch)
    {
        for (x = x1; x < x2; x++)
        {
            resultat =
                VScreen[offsetY - pitch + x] +
                VScreen[offsetY + pitch + x] +
                VScreen[offsetY + x + 1] +
                VScreen[offsetY + x - 1];
            VScreen[offsetY + x] = resultat >> 2;
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
    afficherLogo(VScreen->w - LOGO_W, 0, VScreen);
}

void afficherLogoTile(SDL_Surface *VScreen)
{
    int nbX = VScreen->w / LOGO_W + 1;
    int nbY = VScreen->h / LOGO_H + 1;

    SDL_FillRect(VScreen, NULL, 0);
    for (int incX = 0; incX < nbX; incX++)
    {
        for (int incY = 0; incY < nbY; incY++)
        {
            afficherLogo(incX * LOGO_W + incX * 10, incY * LOGO_H + incY * 10, VScreen);
        }
    }
}

void afficherLogoCenter(SDL_Surface *VScreen)
{
    SDL_FillRect(VScreen, NULL, 0);
    afficherLogo((VScreen->w / 2) - (LOGO_W / 2), (VScreen->h / 2) - (LOGO_H / 2), VScreen);
}

void afficherLogo(int x, int y, SDL_Surface *VScreen)
{
    for (int incX = 0; incX < LOGO_W; incX++)
    {
        for (int incY = 0; incY < LOGO_H; incY++)
        {
            if (c_logo_map[incY * LOGO_W + incX] != 0x00)
            {
                putPixel(incX + x, incY + y, 255, false, VScreen);
            }
        }
    }
}

void afficherPalette(SDL_Surface *VScreen)
{
    for (int incX = 0; incX < 40; incX++)
    {
        for (int incY = 0; incY < VScreen->h; incY++)
        {
            putPixel(incX, incY, incY * 256 / VScreen->h, false, VScreen);
        }
    }
}

void calculerInfosTimer(char *texte, clock_t t1, clock_t t2, long nbFrame)
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
    snprintf(texte, 100, "FPS : %ld, moyenne: %lf", (long)fps, moyenne);
}

void afficherBoard(CONTEXTE *contexte, SDL_Surface *surface, clock_t t1, clock_t t2, long nbFrame)
{
    char texte[100];
    SDL_Rect dstRect = {100, 0, 50, 200};

    snprintf(texte, sizeof(texte), "Nombre Particule:%d", contexte->nombreMeteor);

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
    int taille = (contexte->largeur < contexte->hauteur ? contexte->largeur : contexte->hauteur) / 2;
    for (int x = 0; x < contexte->largeur; x++)
    {
        float deltaX = (contexte->largeur / 2) - x;
        float longX = deltaX * deltaX;
        for (int y = 0; y < contexte->hauteur; y++)
        {
            float deltaY = (contexte->hauteur / 2) - y;
            float longY = deltaY * deltaY;
            float longueur = sqrt(longX + longY);
            int hauteur = 0;
            if (longueur <= taille)
            {
                hauteur = 256 - (longueur * 256 / taille);
            }
            putPixel(x, y, hauteur, false, contexte->phongmap);
        }
    }
}

void drawShadow(CONTEXTE *contexte, int x, int y, int offset)
{
    float calcOffset = 1 / (float)offset;
    float hauteurLumiereCache = 5 - calcOffset / 4;
    float hauteurCache = 5;
    float hauteurLumiere = hauteurCache + hauteurLumiereCache;
    float rapportHauteur = hauteurLumiereCache / hauteurLumiere;
    int posX, posY;
    int offsetY = 0;
    int larg = contexte->largeur;
    int haut = contexte->hauteur;

    uint8_t *cache = (uint8_t *)contexte->cache->pixels;
    uint8_t *dest = (uint8_t *)contexte->surface->pixels;

    for (int incY = 0; incY < haut; incY++)
    {
        float longY = y - incY;
        float longYCache = longY * rapportHauteur;
        posY = y - longYCache - calcOffset;

        for (int incX = 0; incX < larg; incX++)
        {
            float longX = x - incX;
            float longXCache = longX * rapportHauteur;
            posX = x - longXCache - calcOffset;

            if (cache[offsetY + incX] != 0x00)
            {
                *dest = 255;
            }
            else
            {
                int offsetCache = posY * larg + posX;
                if (offsetCache >= 0 && cache[offsetCache] != 0x00)
                {
                    uint8_t couleurCache = cache[offsetCache];
                    if (couleurCache < 100)
                    {
                        couleurCache = 0;
                    }
                    else
                    {
                        couleurCache -= 100;
                    }
                    uint8_t couleur = *dest;
                    if (couleurCache > couleur)
                    {
                        couleur = 0;
                    }
                    else
                    {
                        couleur = couleur - couleurCache;
                    }

                    *dest = couleur;
                }
            }

            dest++;
        }
        offsetY += larg;
    }
}

void drawBumpMapping(CONTEXTE *contexte, int x, int y)
{
    int xdelta, ydelta;
    int xtemp, ytemp;
    int incY, incX;
    int lx, ly;
    int larg = contexte->largeur;
    int haut = contexte->hauteur;
    int offsetY = larg;
    uint8_t *source = (uint8_t *)contexte->bump->pixels;
    uint8_t *dest = (uint8_t *)contexte->surface->pixels;
    uint8_t *phonglightmap = (uint8_t *)contexte->phongmap->pixels;

    ly = -(y - contexte->hauteur / 2);
    for (incY = 1; incY < haut - 2; incY++)
    {
        lx = -(x - contexte->largeur / 2);

        for (incX = 0; incX < larg; incX++)
        {
            xdelta = ((source[offsetY + (incX - 1)] - source[offsetY + incX]));
            ydelta = ((source[offsetY + incX] - source[offsetY + larg + incX]));
            lx++;

            xtemp = xdelta + lx;
            ytemp = ydelta + ly;
            if (xtemp < 0 || xtemp >= larg || ytemp < 0 || ytemp >= haut)
            {
                *dest++ = 0;
            }
            else
            {
                *dest++ = phonglightmap[ytemp * larg + xtemp];
            }
        }
        offsetY += larg;
        ly++;
    }
}

void drawGlass(CONTEXTE *contexte)
{
    int xdelta, ydelta;
    int xtemp, ytemp;
    int incY, incX;
    int larg = contexte->largeur;
    int haut = contexte->hauteur;
    int offsetY = larg;
    uint8_t *source = (uint8_t *)contexte->bump->pixels;
    uint8_t *dest = (uint8_t *)contexte->surface->pixels;
    uint8_t *cache = (uint8_t *)contexte->cache->pixels;

    for (incY = 1; incY < haut - 2; incY++)
    {
        for (incX = 0; incX < larg; incX++)
        {
            xdelta = ((source[offsetY + (incX - 1)] - source[offsetY + incX]));
            ydelta = ((source[offsetY + incX] - source[offsetY + larg + incX]));

            xtemp = incX - xdelta;
            ytemp = incY - ydelta;
            if (xtemp < 0 || xtemp >= larg || ytemp < 0 || ytemp >= haut)
            {
                *dest++ = 0;
            }
            else
            {
                *dest++ = cache[ytemp * larg + xtemp];
            }
        }
        offsetY += larg;
    }
}