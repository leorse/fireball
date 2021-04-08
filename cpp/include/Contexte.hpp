#pragma once

#include <list>

#include "Particule.hpp"

class Contexte
{
private:
    bool programAlive;

public:
    static const int HAUTEUR = 1000;
    static const int LARGEUR = 1000;
    static const int NB_PARTICULE = 1;

    std::list<Particule*> particules;

    Particule* factoryParticule();

    bool isProgramAlive();
    void stopProgram();

    void growParticules();
    void drawParticules();

    Contexte();
    ~Contexte();
};