#pragma once



#include "PartManager.hpp"

class App
{
private:
    bool programAlive;
    PartManager partManager;
    std::default_random_engine re;

public:
    static const int HAUTEUR = 1000;
    static const int LARGEUR = 1000;

    bool isProgramAlive();
    void stopProgram();

    void growParticules();
    void drawParticules();

    App();
    ~App();
};