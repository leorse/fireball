#pragma once

#include <SDL2/SDL.h>
#include "PartManager.hpp"
#include "Drawer.hpp"

class App
{
private:
    bool programAlive;
    PartManager partManager;
    std::default_random_engine re;
    int mousePosition[2];

    bool drawPalette;


    bool drawBoard;
    bool drawBlur;
    bool drawLogo;

    enum MODE_EFFECT
    {
        FIREBALL, LIGHT, SHADOW, GLASS
    };

    enum MODE_EFFECT modeEffect;

    Drawer drawer;
public:
    void keyPress(SDL_KeyboardEvent *kE);

    bool isDrawPalette() const;

    void setDrawPalette(bool drawPalette);

    bool isDrawBoard() const;

    void setDrawBoard(bool drawBoard);

    bool isDrawBlur() const;

    void setDrawBlur(bool drawBlur);

    bool isDrawLogo() const;

    void setDrawLogo(bool drawLogo);

    MODE_EFFECT getModeEffect() const;

    void setModeEffect(MODE_EFFECT modeEffect);

    void switchMode();


    const int *getMousePosition() const;

    void setMousePosition(int, int);

    static const int HAUTEUR = 1000;
    static const int LARGEUR = 1000;

    bool isProgramAlive();

    void stopProgram();

    void growParticules();

    void drawParticules();

    void render();

    void mousePress(SDL_MouseButtonEvent mbe);

    App();

    ~App();
};