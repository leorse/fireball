#pragma once

#include <stdio.h>
#include <stdbool.h>
#include <time.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

#include "contexte.h"
#include "particule.h"

#define L 1000
#define H 1000
#define RANDOM(n) (((float)rand() / (float)RAND_MAX) * (n))

#define PTX (L / 2)
#define PTY (H / 2)

enum MODE_EFFECT
{
    FIREBALL,
    LIGHT
};

typedef struct
{
    PARTICULE *premier;
    PARTICULE *dernier;
    int nombreMeteor;
    bool *Sprites[MAX_TAILLE];
    SDL_Color listeCouleur[256];
    TTF_Font *font;

    SDL_Surface *surface;
    SDL_Surface *phongmap;
    SDL_Surface *bump;
    SDL_Surface *cache;

    bool drawPalette;
    bool drawBoard;
    bool drawBlur;
    enum MODE_EFFECT mode;

} CONTEXTE;


void initContexte(CONTEXTE *);
void detruireContexte(CONTEXTE *contexte);
PARTICULE *initElmt(CONTEXTE* contexte);
void DoPalette(SDL_Color *Palette);
void InitSprite(bool *Sprites[]);
void drawSprite(bool *Sprites[]);
void addParticule(CONTEXTE* contexte, int x, int y);
PARTICULE* removeParticule(CONTEXTE* contexte, PARTICULE* particule);

//GUI
void switchPalette(CONTEXTE* );
void switchBlur(CONTEXTE* );
void switchBoard(CONTEXTE* contexte);
void switchMode(CONTEXTE* contexte);

