#pragma once
/*#ifndef AFFICHAGE_H
#define AFFICHAGE_H*/

#include <SDL2/SDL.h>

#include "particule.h"
#include "contexte.h"

#define TAILLE_LUMIERE 500

void putPixel(int x, int y, int couleur, SDL_Surface *surface);
void blur(int x1, int y1, int x2, int y2, SDL_Surface *surface);
void drawParticule(SDL_Surface *VScreen, bool *Sprites[MAX_TAILLE], PARTICULE *particule);
void afficherPalette(SDL_Surface *VScreen);
void afficherTexte(char* texte, CONTEXTE* contexte, SDL_Surface *surface, SDL_Rect rectDest);
void afficherBoard(CONTEXTE *contexte, SDL_Surface *surface, clock_t t1, clock_t t2, long nbFrame);
void afficherLogo(SDL_Surface *VScreen);
char* calculerInfosTimer(char* texte, clock_t t1, clock_t t2, long nbFrame);
void initialiserLumiere(CONTEXTE* contexte);
void drawBumpMapping(CONTEXTE *contexte, int x, int y);

//#endif