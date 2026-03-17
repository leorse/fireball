#pragma once

#include <stdio.h>
#include <stdbool.h>
#include <time.h>

/* OpenGL must be included before SDL to avoid redefinition warnings */
#include <GL/glew.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

#include "particule.h"

#define RANDOM(n) (((float)rand() / (float)RAND_MAX) * (n))

enum MODE_EFFECT
{
    FIREBALL,
    LIGHT,
    SHADOW,
    GLASS
};

typedef struct
{
    PARTICULE *premier;
    PARTICULE *dernier;
    int nombreMeteor;
    bool *Sprites[MAX_TAILLE];
    SDL_Color listeCouleur[256];
    TTF_Font *font;

    /* Window & GL context */
    SDL_Window    *window;
    SDL_GLContext  glContext;

    /* CPU surfaces (particles + effects computed on CPU) */
    SDL_Surface *bump;   /* 8-bit heightmap drawn to by particles */
    SDL_Surface *cache;  /* 8-bit background for SHADOW/GLASS modes */
    SDL_Surface *hud;    /* RGBA32 overlay for text/palette bar */

    /* OpenGL textures */
    GLuint texHeightmap; /* R8, uploaded from bump every frame */
    GLuint texCache;     /* R8, uploaded when cacheTextureNeedsUpdate */
    GLuint texPalette;   /* RGBA8 256x1, fire colour palette */
    GLuint texHUD;       /* RGBA8, uploaded when HUD is visible */

    /* GL programs: [0]=fireball [1]=light [2]=shadow [3]=glass [4]=hud */
    GLuint programs[5];
    GLuint quadVAO;
    GLuint quadVBO;

    bool cacheTextureNeedsUpdate;

    bool drawPalette;
    bool drawBoard;
    bool drawBlur;
    bool drawLogo;
    enum MODE_EFFECT mode;

    int largeur;
    int hauteur;

} CONTEXTE;


void      initContexte(CONTEXTE *, int largeur, int hauteur);
void      redimensionnerContexte(CONTEXTE *contexte, int nouvLargeur, int nouvHauteur);
void      detruireContexte(CONTEXTE *contexte);
PARTICULE *initElmt(CONTEXTE *contexte);
void      doPalette(SDL_Color *Palette);
void      initSprite(bool *Sprites[]);
void      drawSprite(bool *Sprites[]);
void      addParticule(CONTEXTE *contexte, int nombre, int x, int y);
PARTICULE *removeParticule(CONTEXTE *contexte, PARTICULE *particule);

/* GUI key handlers */
void switchPalette(CONTEXTE *);
void switchBlur(CONTEXTE *);
void switchBoard(CONTEXTE *);
void switchLogo(CONTEXTE *);
void switchMode(CONTEXTE *contexte);
