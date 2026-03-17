#pragma once

#include <GL/glew.h>
#include "contexte.h"

bool  initGL(CONTEXTE *contexte);
void  detruireGL(CONTEXTE *contexte);
void  redimensionnerGL(CONTEXTE *contexte, int nouvLargeur, int nouvHauteur);
void  mettreAJourPalette(CONTEXTE *contexte);
