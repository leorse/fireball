#pragma once

#include <time.h>
#include <SDL2/SDL.h>

#include "particule.h"
#include "contexte.h"

/* Low-level surface drawing (still CPU, used for particle rendering) */
void putPixel(int x, int y, int couleur, bool force, SDL_Surface *surface);
void blur(int x1, int y1, int x2, int y2, SDL_Surface *surface);
void drawParticule(SDL_Surface *VScreen, bool *Sprites[MAX_TAILLE], PARTICULE *particule);

/* Logo helpers (draw to 8-bit surface) */
void afficherLogoTopRight(SDL_Surface *VScreen);
void afficherLogoCenter(SDL_Surface *VScreen);
void afficherLogoTile(SDL_Surface *VScreen);
void afficherLogo(int x, int y, SDL_Surface *VScreen);

/* HUD helpers (draw to RGBA32 hud surface) */
void afficherPaletteHUD(CONTEXTE *contexte);
void afficherBoardHUD(CONTEXTE *contexte, clock_t t1, clock_t t2, long nbFrame);

/* Main GPU render call — uploads textures, sets uniforms, draws quad + HUD */
void rendreGPU(CONTEXTE *contexte, int mouseX, int mouseY,
               clock_t t1, clock_t t2, long nbFrame);
