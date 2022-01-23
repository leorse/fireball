#include <cstdlib>
#include <ctime>
#include <iostream>

#include "App.hpp"

App::App()
{
    this->programAlive = true;
    re.seed(time(0));
    this->partManager.setSeed(re);
}

App::~App()
{
    cout << "app détruit" << endl;
}

void App::mousePress(SDL_MouseButtonEvent mbe)
{
    if (mbe.button == SDL_BUTTON_LEFT)
    {
        this->partManager.addParticules( 50, mbe.x, mbe.y);
    }
}

void App::growParticules()
{

}


void App::render()
{
    this->partManager.growParticules();
    this->partManager.drawParticules();
    this->drawer.afficherLogoTopRight();
}

bool App::isProgramAlive()
{
    return this->programAlive;
}

void App::stopProgram()
{
    this->programAlive = false;
}

void App::drawParticules()
{
}

const int *App::getMousePosition() const
{
    return mousePosition;
}

void App::setMousePosition(int x, int y)
{
    this->mousePosition[0] = x;
    this->mousePosition[1] = y;
}

App::MODE_EFFECT App::getModeEffect() const
{
    return modeEffect;
}

void App::setModeEffect(App::MODE_EFFECT modeEffect)
{
    App::modeEffect = modeEffect;
}

bool App::isDrawPalette() const
{
    return drawPalette;
}

void App::setDrawPalette(bool drawPalette)
{
    App::drawPalette = drawPalette;
}

bool App::isDrawBoard() const
{
    return drawBoard;
}

void App::setDrawBoard(bool drawBoard)
{
    App::drawBoard = drawBoard;
}

bool App::isDrawBlur() const
{
    return drawBlur;
}

void App::setDrawBlur(bool drawBlur)
{
    App::drawBlur = drawBlur;
}

bool App::isDrawLogo() const
{
    return drawLogo;
}

void App::setDrawLogo(bool drawLogo)
{
    App::drawLogo = drawLogo;
}

void App::switchMode()
{
    if (this->modeEffect == FIREBALL)
    {
      /*  initialiserLumiere(contexte);
        SDL_BlitSurface(contexte->surface, NULL, contexte->bump, NULL);
        this->modeEffect = LIGHT;*/
        //drawBumpMapping(contexte);
    }
    else if (this->modeEffect == LIGHT)
    {
       //copier le cache
        /*afficherLogoCenter(contexte->cache);
        blur(1, 1, L - 1, H - 1, contexte->cache);
        blur(1, 1, L - 1, H - 1, contexte->cache);
        blur(1, 1, L - 1, H - 1, contexte->cache);*/
        this->modeEffect = SHADOW;
    }
    else if (this->modeEffect == SHADOW)
    {
        this->modeEffect = GLASS;
        //afficherLogoTile(contexte->cache);
    }
    else if (this->modeEffect == GLASS)
    {
        this->modeEffect = FIREBALL;
    }
}

void App::keyPress(SDL_KeyboardEvent *kE)
{
    SDL_Keysym ks = kE->keysym;
    if (kE->keysym.scancode == SDL_SCANCODE_P)
    {
        this->drawPalette = !this->drawPalette;
    }
    if (kE->keysym.scancode == SDL_SCANCODE_B)
    {
        this->drawBoard = !this->drawBoard;
    }
    if (kE->keysym.scancode == SDL_SCANCODE_F)
    {
        this->drawBlur = !this->drawBlur;
    }
    if (kE->keysym.scancode == SDL_SCANCODE_L)
    {
        this->drawLogo = !this->drawLogo;
    }
    if (kE->keysym.scancode == SDL_SCANCODE_M)
    {
        this->switchMode();
    }
}
