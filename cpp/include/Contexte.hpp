#pragma once



#include "PartManager.hpp"

class Contexte
{
private:
    bool programAlive;
    PartManager partManager;

public:
    static const int HAUTEUR = 1000;
    static const int LARGEUR = 1000;




    bool isProgramAlive();
    void stopProgram();

    void growParticules();

    Contexte();
    ~Contexte();
};