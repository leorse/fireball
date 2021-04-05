#pragma once
/*#ifndef INITSDL_H
#define INITSDL_H*/

#include <SDL2/SDL.h>

#include <stdbool.h>
#include "contexte.h"


bool initSDL(CONTEXTE* , SDL_Surface **, SDL_Renderer **);

//#endif