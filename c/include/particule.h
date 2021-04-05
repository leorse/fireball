#pragma once
/*#ifndef PARTICULE_H
#define PARTICULE_H*/

#include <SDL2/SDL.h>
#include <stdbool.h>
#include <math.h>


#define NB_BOULETTE 200
#define MAX_VITESSE 45
#define MAX_TAILLE 6
#define MAX_DUREE 25
#define PI 3.14159

typedef struct _PARTICULE
{
    struct _PARTICULE *suivant;
    struct _PARTICULE *precedent;
    float x;
    float y;
    float tps;
    float dir;
    float poids;
    int vitesse;
    int taille;
    int vie;
    double dX;
    double dY;

    bool ephemere;

    float refX, refY;

    bool (*Grower)(struct _PARTICULE *);
    void (*Mover)(struct _PARTICULE *);
    void (*Drawer)(struct _PARTICULE *, SDL_Surface *, bool *[]);

} PARTICULE;

bool faireVivre(PARTICULE *);
void moveTour(PARTICULE *);
void moveClassic(PARTICULE *);
void ParticuleFactory(PARTICULE *);
void afficheurSimple(PARTICULE *, SDL_Surface *, bool *[]);
void afficheurTrainee(PARTICULE *this, SDL_Surface *, bool *[]);
void drawline(int , int , int , int , int , SDL_Surface *);

//#endif