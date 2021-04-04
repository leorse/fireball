#pragma once
/*#ifndef AFFICHAGE_H
#define AFFICHAGE_H*/

#include <SDL2/SDL.h>

#include "particule.h"

void putPixel(int x, int y, int couleur, SDL_Surface *surface);
void blur(int x1, int y1, int x2, int y2, SDL_Surface *surface);
void drawParticule(SDL_Surface *VScreen, bool *Sprites[MAX_TAILLE], PARTICULE *particule);
void afficherPalette(SDL_Surface *VScreen);

//#endif