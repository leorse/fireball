#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "linkedList.h"
#include "contexte.h"
#include "particule.h"
#include "initSDL.h"
#include "affichage.h"
#include "shader.h"

void initContexte(CONTEXTE *contexte, int largeur, int hauteur)
{
    contexte->largeur   = largeur;
    contexte->hauteur   = hauteur;
    contexte->nombreMeteor = 0;
    doPalette(contexte->listeCouleur);

    contexte->window    = NULL;
    contexte->glContext = NULL;
    contexte->bump      = NULL;
    contexte->cache     = NULL;
    contexte->hud       = NULL;

    contexte->texHeightmap = 0;
    contexte->texCache     = 0;
    contexte->texPalette   = 0;
    contexte->texHUD       = 0;
    memset(contexte->programs, 0, sizeof(contexte->programs));
    contexte->quadVAO = 0;
    contexte->quadVBO = 0;
    contexte->cacheTextureNeedsUpdate = false;

    contexte->dernier = initElmt(contexte);
    initSprite(contexte->Sprites);

    contexte->drawPalette = false;
    contexte->drawBoard   = false;
    contexte->drawBlur    = true;
    contexte->drawLogo    = false;
    contexte->mode        = FIREBALL;
}

void addParticule(CONTEXTE *contexte, int nombre, int x, int y)
{
    for (int i = 0; i < nombre; i++)
    {
        PARTICULE *ptr = (PARTICULE *)__CT_creerElement(sizeof(PARTICULE), contexte->dernier);
        ParticuleFactory(ptr);
        ptr->refX      = (float)x;
        ptr->refY      = (float)y;
        ptr->ephemere  = true;
        ptr->explosive = false;
        contexte->dernier = ptr;
        contexte->nombreMeteor++;
    }
}

PARTICULE *removeParticule(CONTEXTE *contexte, PARTICULE *particule)
{
    PARTICULE *retour = NULL;
    if (particule == contexte->premier)
    {
        contexte->premier = contexte->premier->suivant;
        retour = contexte->premier;
    }
    else if (particule == contexte->dernier)
    {
        contexte->dernier = contexte->dernier->precedent;
        retour = contexte->dernier;
    }
    else
    {
        retour = particule->suivant;
    }
    __CT_libererUnElement(particule);
    contexte->nombreMeteor--;
    return retour;
}

PARTICULE *initElmt(CONTEXTE *contexte)
{
    PARTICULE **lst = &(contexte->premier);
    srand(time(NULL));

    *lst = (PARTICULE *)__CT_creerElement(sizeof(PARTICULE), NULL);
    PARTICULE *ptr = *lst;
    ParticuleFactory(ptr);
    ptr->refX = (float)(contexte->largeur / 2);
    ptr->refY = (float)(contexte->hauteur / 2);
    contexte->nombreMeteor++;

    for (int i = 0; i < NB_BOULETTE - 1; i++)
    {
        ptr = (PARTICULE *)__CT_creerElement(sizeof(PARTICULE), ptr);
        ParticuleFactory(ptr);
        ptr->refX = (float)(contexte->largeur / 2);
        ptr->refY = (float)(contexte->hauteur / 2);
        contexte->nombreMeteor++;
    }
    return ptr;
}

void detruireContexte(CONTEXTE *contexte)
{
    __CT_libererElements(contexte->premier);

    for (int i = 0; i < MAX_TAILLE; i++)
        free(contexte->Sprites[i]);

    if (contexte->font)  TTF_CloseFont(contexte->font);
    if (contexte->bump)  SDL_FreeSurface(contexte->bump);
    if (contexte->cache) SDL_FreeSurface(contexte->cache);
    if (contexte->hud)   SDL_FreeSurface(contexte->hud);

    detruireGL(contexte);

    if (contexte->window) SDL_DestroyWindow(contexte->window);
    TTF_Quit();
    SDL_Quit();
}

void redimensionnerContexte(CONTEXTE *contexte, int nouvLargeur, int nouvHauteur)
{
    SDL_FreeSurface(contexte->bump);
    SDL_FreeSurface(contexte->cache);
    SDL_FreeSurface(contexte->hud);

    contexte->largeur  = nouvLargeur;
    contexte->hauteur  = nouvHauteur;

    contexte->bump = SDL_CreateRGBSurface(SDL_SWSURFACE,
                                          nouvLargeur, nouvHauteur,
                                          8, 0, 0, 0, 0);
    contexte->cache = SDL_CreateRGBSurface(SDL_SWSURFACE,
                                           nouvLargeur, nouvHauteur,
                                           8, 0, 0, 0, 0);
    contexte->hud = SDL_CreateRGBSurfaceWithFormat(0,
                                                   nouvLargeur, nouvHauteur,
                                                   32, SDL_PIXELFORMAT_RGBA32);

    /* Resize GL textures */
    redimensionnerGL(contexte, nouvLargeur, nouvHauteur);

    /* Regenerate content that depends on dimensions */
    if (contexte->mode == SHADOW)
    {
        afficherLogoCenter(contexte->cache);
        blur(1, 1, nouvLargeur - 1, nouvHauteur - 1, contexte->cache);
        blur(1, 1, nouvLargeur - 1, nouvHauteur - 1, contexte->cache);
        blur(1, 1, nouvLargeur - 1, nouvHauteur - 1, contexte->cache);
        contexte->cacheTextureNeedsUpdate = true;
    }
    else if (contexte->mode == GLASS)
    {
        afficherLogoTile(contexte->cache);
        contexte->cacheTextureNeedsUpdate = true;
    }
}

void doPalette(SDL_Color *Palette)
{
    for (int i = 0; i < 128; i++)
    {
        Palette[i].r = i;
        Palette[i].g = 0;
        Palette[i].b = 0;
        Palette[i].a = 255;
    }
    for (int i = 128; i < 256; i++)
    {
        Palette[i].r = i;
        Palette[i].g = (i - 128) * 2;
        Palette[i].b = 0;
        Palette[i].a = 255;
    }
    Palette[255].r = 255;
    Palette[255].g = 255;
    Palette[255].b = 255;
    Palette[255].a = 255;
}

void initSprite(bool *Sprites[MAX_TAILLE])
{
    for (int i = 0; i < MAX_TAILLE; i++)
    {
        int taille = (i + 1) * (i + 1);
        Sprites[i] = (bool *)malloc(taille * sizeof(bool));
        if (!Sprites[i])
        {
            fprintf(stderr, "Erreur malloc sprite %d\n", i);
            exit(EXIT_FAILURE);
        }
        bool *sprite = Sprites[i];

        if (i == 0)
        {
            sprite[0] = true;
        }
        if (i == 1)
        {
            bool t[2][2] = {{true, false}, {false, true}};
            memcpy(sprite, t, sizeof(t));
        }
        if (i == 2)
        {
            bool t[3][3] = {
                {false, true, false},
                {true,  true, true},
                {false, true, false},
            };
            memcpy(sprite, t, sizeof(t));
        }
        if (i == 3)
        {
            bool t[4][4] = {
                {false, true,  false, false},
                {false, true,  true,  true},
                {true,  true,  true,  false},
                {false, false, true,  false}};
            memcpy(sprite, t, sizeof(t));
        }
        if (i == 4)
        {
            bool t[5][5] = {
                {false, false, true,  false, false},
                {false, true,  true,  true,  false},
                {true,  true,  true,  true,  true},
                {false, true,  true,  true,  false},
                {false, false, true,  false, false}};
            memcpy(sprite, t, sizeof(t));
        }
        if (i == 5)
        {
            bool t[6][6] = {
                {false, false, true,  false, false, false},
                {false, true,  true,  true,  false, false},
                {false, true,  true,  true,  true,  true},
                {true,  true,  true,  true,  true,  false},
                {false, true,  true,  true,  true,  false},
                {false, false, false, true,  false, false}};
            memcpy(sprite, t, sizeof(t));
        }
    }
}

void drawSprite(bool *Sprites[MAX_TAILLE])
{
    for (int inc = 0; inc < MAX_TAILLE; inc++)
    {
        bool *sprite = Sprites[inc];
        printf("--- Sprite %2d:\n", inc + 1);
        int taille = inc + 1;
        for (int incX = 0; incX < taille; incX++)
        {
            for (int incY = 0; incY < taille; incY++)
                printf("%c,", sprite[incX * taille + incY] ? '1' : '0');
            printf("\n");
        }
        printf("\n");
    }
}

void switchPalette(CONTEXTE *contexte) { contexte->drawPalette = !contexte->drawPalette; }
void switchBoard(CONTEXTE *contexte)   { contexte->drawBoard   = !contexte->drawBoard;   }
void switchBlur(CONTEXTE *contexte)    { contexte->drawBlur    = !contexte->drawBlur;    }
void switchLogo(CONTEXTE *contexte)    { contexte->drawLogo    = !contexte->drawLogo;    }

void switchMode(CONTEXTE *contexte)
{
    if (contexte->mode == FIREBALL)
    {
        contexte->mode = LIGHT;
        /* Copy current bump into heightmap for lighting (already uploaded each frame) */
        SDL_BlitSurface(contexte->bump, NULL, contexte->cache, NULL);
    }
    else if (contexte->mode == LIGHT)
    {
        afficherLogoCenter(contexte->cache);
        blur(1, 1, contexte->largeur - 1, contexte->hauteur - 1, contexte->cache);
        blur(1, 1, contexte->largeur - 1, contexte->hauteur - 1, contexte->cache);
        blur(1, 1, contexte->largeur - 1, contexte->hauteur - 1, contexte->cache);
        contexte->cacheTextureNeedsUpdate = true;
        contexte->mode = SHADOW;
    }
    else if (contexte->mode == SHADOW)
    {
        afficherLogoTile(contexte->cache);
        contexte->cacheTextureNeedsUpdate = true;
        contexte->mode = GLASS;
    }
    else if (contexte->mode == GLASS)
    {
        contexte->mode = FIREBALL;
    }
}
