#pragma once

#include <vector>
#include <array>

#include "Particule.hpp"
#include "Logo.hpp"

class Drawer
{
private:
    std::vector<std::vector<bool>> sprites;

public:
    Drawer();

    void putPixel(int x, int y, int couleur);
    void blur(int x1, int y1, int x2, int y2);
    void drawline(int x0, int y0, int x1, int y1, int couleur);
    void afficherLogo(int x, int y);
    void afficherLogoTopRight();
    void afficheurSimple(Particule *);
    void afficheurTrainee(Particule *part);
    void initSprites();
    //void afficherBoard(CONTEXTE *contexte/*, clock_t t1, clock_t t2, long nbFrame*/);

private:
};